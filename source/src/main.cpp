// THIS IS THE FILE YOU MUST START FROM!

// This has been adapted from the Vulkan tutorial
#include <sstream>

#include <json.hpp>

#include "modules/Starter.hpp"
#include "modules/TextMaker.hpp"
#include "modules/Scene.hpp"

// The uniform buffer object used in this example
struct UniformBufferObject {
	alignas(16) glm::mat4 mvpMat;
	alignas(16) glm::mat4 mMat;
};

struct GlobalUniformBufferObject {
	alignas(16) glm::vec4 eyePos;
	alignas(16) glm::vec4 ambientColor;
	alignas(16) glm::vec4 directionalLightDir;
	alignas(16) glm::vec4 directionalLightColor;
	alignas(16) glm::vec4 pointLightPositions[6];
	alignas(16) glm::vec4 pointLightColors[6];
	alignas(16) glm::vec4 materialParams;
	alignas(16) glm::ivec4 lightCounts;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 UV;
};

// MAIN !

class DungeonTavernNPCApp : public BaseProject {
	protected:
	// Here you list all the Vulkan objects you need:
	
	// Descriptor Layouts [what will be passed to the shaders]
	DescriptorSetLayout DSLlocal, DSLglobal;

	// Vertex formants, Pipelines [Shader couples] and Render passes
	VertexDescriptor VD;
	RenderPass RP;
	Pipeline P;

	// Models, textures and Descriptors (values assigned to the uniforms)
	DescriptorSet DSglobal;

	// To support loading assets from a scene.json file
	Scene SC;
	std::vector<VertexDescriptorRef>  VDRs;
	std::vector<TechniqueRef> PRs;

	// to provide textual feedback
	TextMaker txt;
	
	// Other application parameters
	float Ar;	// Aspect ratio

	glm::mat4 ViewPrj;
	glm::mat4 View;

	// Camera parameters
	// Initial position inside the tavern
	glm::vec3 cameraPos = glm::vec3(0.0f, 1.7f, 2.6f);

	float yaw = 0.0f;
	float pitch = 0.0f;

	const float moveSpeed = 3.0f;
	const float verticalSpeed = 1.5f;
	const float rotateSpeed = 2.0f;
	const int cameraRoomLimitFlag = 1; // 1 = stay inside the room, 0 = free debug camera.
	const glm::vec3 cameraRoomMin = glm::vec3(-4.55f, 0.35f, -11.25f);
	const glm::vec3 cameraRoomMax = glm::vec3(4.55f, 4.25f, 4.75f);
	const float greeterInteractionRadius = 2.0f;
	const float bartenderInteractionRadius = 3.4f;

	// NPC greeter and bartender
	glm::vec3 greeterPos = glm::vec3(3.0f, 1.05f, 2.9f);
	glm::vec3 bartenderPos = glm::vec3(0.0f, 1.2f, -10.2f);

	bool registered = false;
	bool registerHintShown = false;

	bool orderHintShown = false;
	bool orderCompleted = false;
	bool interactionKeyWasPressed = false;
	bool drinkChoiceKeyWasPressed = false;
	bool bartenderMenuOpen = false;
	std::string message = "";
	std::string displayedInteractionMessage = "";
	std::string greeterFeedback = "";
	std::string selectedDrink = "";

	// Here you set the main application parameters
	void setWindowParameters() {
		// window size, titile and initial background
		windowWidth = 1600;
		windowHeight = 900;
		windowTitle = "Zijie & Zhiying DungeonTavernNPC";
    	windowResizable = GLFW_TRUE;
		
		// Initial aspect ratio
		Ar = 4.0f / 3.0f;
	}
	
	// What to do when the window changes size
	void onWindowResize(int w, int h) {
		std::cout << "Window resized to: " << w << " x " << h << "\n";
		// Avoid division by zero when the window is minimized.
		if (w == 0 || h == 0)
			return;
		Ar = (float)w / (float)h;
		// Update Render Pass
		RP.width = w;
		RP.height = h;
		
		// updates the textual output
		txt.resizeScreen(w, h);
	}
	
	// Here you load and setup all your Vulkan Models and Texutures.
	// Here you also create your Descriptor set layouts and load the shaders for the pipelines
	void localInit() {
		// Descriptor Layouts [what will be passed to the shaders]
		DSLlocal.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 1},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0, 1}
				  });
		DSLglobal.init(this, {
					// this array contains the binding:
					// first  element : the binding number
					// second element : the type of element (buffer or texture)
					// third  element : the pipeline stage where it will be used
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, sizeof(GlobalUniformBufferObject), 1}
				  });
		VD.init(this, {
				  {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}
				}, {
				  {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, pos),
				         sizeof(glm::vec3), POSITION},
				  {0, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, norm),
				         sizeof(glm::vec3), NORMAL},
				  {0, 2, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, UV),
				         sizeof(glm::vec2), UV}
				});

		// initializes the render passes
		RP.init(this);
		// Dark background for the closed indoor tavern scene.
		RP.properties[0].clearValue = {0.025f, 0.022f, 0.020f, 1.0f};

		// Pipelines [Shader couples]
		// The last array, is a vector of pointer to the layouts of the sets that will
		// be used in this pipeline. The first element will be set 0, and so on..
		
		P.init(this, &VD, "shaders/tavern_lit.vert.spv",
						  "shaders/tavern_lit.frag.spv",
						  {&DSLglobal, &DSLlocal});


		// sets the size of the Descriptor Set Pool (it MUST be done before loading the scene)
		DPSZs.uniformBlocksInPool = 2;
		DPSZs.texturesInPool = 1;
		DPSZs.setsInPool = 2;

		// to support scene
		VDRs.resize(1);
		VDRs[0].init("VDposNormUV",  &VD);

		PRs.resize(1);
		PRs[0].init("BlinnPhong", {
							{&P, {//Pipeline and DSL for the main pass
							 /*DSLglobal*/{},
							 /*DSLlocal*/{
									/*t0*/{true,  0, {}}
								  }
								 }
								}
						  }, /*TotalNtextures*/1, &VD);

		if(SC.init(this, 1, VDRs, PRs, "assets/scenes/scene.json") != 0) {
			std::cout << "ERROR LOADING THE SCENE\n";
			exit(0);
		}

		// initializes the textual output
		txt.init(this, windowWidth, windowHeight);

		// submits the main command buffer
		submitCommandBuffer("main", 0, populateCommandBufferAccess, this);

		// Prepares for showing the FPS count
		txt.print(1.0f, 1.0f, "FPS:",1,"CO",false,false,true,TAL_RIGHT,TRH_RIGHT,TRV_BOTTOM,{1.0f,0.0f,0.0f,1.0f},{0.8f,0.8f,0.0f,1.0f});

	}
	
	// Here you create your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsInit() {
		// creates the render passes
		RP.create();
		
		// This creates a new pipeline (with the current surface), using its shaders for the provided render pass
		P.create(&RP);
		
		DSglobal.init(this, &DSLglobal, {});
		
		// Here you define the data set
		// If the scene has textures coming from a render pass, the corresponding element of the technique must be
		// updated before calling SC.pipelinesAndDescriptorSetsInit();

		SC.pipelinesAndDescriptorSetsInit();
		txt.pipelinesAndDescriptorSetsInit();
	}

	// Here you destroy your pipelines and Descriptor Sets!
	void pipelinesAndDescriptorSetsCleanup() {
		P.cleanup();

		RP.cleanup();
		
		DSglobal.cleanup();
		
		SC.pipelinesAndDescriptorSetsCleanup();
		txt.pipelinesAndDescriptorSetsCleanup();
	}

	// Here you destroy all the Models, Texture and Desc. Set Layouts you created!
	// You also have to destroy the pipelines
	void localCleanup() {
		DSLlocal.cleanup();
		DSLglobal.cleanup();

		P.destroy();

		RP.destroy();

		SC.localCleanup();
		txt.localCleanup();
	}
	
	// Here it is the creation of the command buffer:
	// You send to the GPU all the objects you want to draw,
	// with their buffers and textures
	static void populateCommandBufferAccess(VkCommandBuffer commandBuffer, int currentImage, void *Params) {
		// Simple trick to avoid having always 'T->'
		// in che code that populates the command buffer!
		DungeonTavernNPCApp *T = (DungeonTavernNPCApp *)Params;
		T->populateCommandBuffer(commandBuffer, currentImage);
	}

	void populateCommandBuffer(VkCommandBuffer commandBuffer, int currentImage) {
		
		// Offscreen pass - always required
		// begin standard pass
		RP.begin(commandBuffer, currentImage);

		SC.populateCommandBuffer(commandBuffer, 0, currentImage);

		RP.end(commandBuffer);
	}

	float horizontalDistance(glm::vec3 a, glm::vec3 b) {
		return glm::distance(glm::vec2(a.x, a.z), glm::vec2(b.x, b.z));
	}

	void updateInteractionText() {
		const int interactionTextId = 3;

		if(message == displayedInteractionMessage) {
			return;
		}

		if(message.empty()) {
			txt.removeText(interactionTextId);
		} else {
			txt.print(-0.95f, -0.95f, message, interactionTextId, "CO", false, true, true,
					  TAL_LEFT, TRH_LEFT, TRV_TOP,
					  {1.0f,1.0f,1.0f,1.0f},
					  {0.0f,0.0f,0.0f,1.0f});
		}

		displayedInteractionMessage = message;
	}

	std::string drinkChoiceMenuText() {
		return "What would you like to drink?\n1. Ale\n2. Red wine\n3. Water";
	}

	std::string drinkNameFromChoice(int choice) {
		if(choice == 1) return "Ale";
		if(choice == 2) return "Red wine";
		if(choice == 3) return "Water";
		return "";
	}

	// Here is where you update the uniforms.
	// Very likely this will be where you will be writing the logic of your application.
	void updateUniformBuffer(uint32_t currentImage) {
		// handle the ESC key to exit the app
		if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// moves the view
		float deltaT = GameLogic();
		
		// Global lighting parameters for the tavern shader.
		GlobalUniformBufferObject gubo{};

		gubo.eyePos = glm::vec4(cameraPos, 1.0f);
		gubo.ambientColor = glm::vec4(0.055f, 0.045f, 0.035f, 1.0f);
		gubo.directionalLightDir = glm::vec4(glm::normalize(glm::vec3(-0.25f, -1.0f, -0.35f)), 0.0f);
		gubo.directionalLightColor = glm::vec4(0.08f, 0.09f, 0.12f, 1.0f);

		gubo.pointLightPositions[0] = glm::vec4(0.0f, 3.35f, -11.15f, 1.0f);  // back wall candle
		gubo.pointLightPositions[1] = glm::vec4(-4.40f, 2.15f, -7.20f, 1.0f); // left rear wall candle
		gubo.pointLightPositions[2] = glm::vec4(4.40f, 2.15f, -7.20f, 1.0f);  // right rear wall candle
		gubo.pointLightPositions[3] = glm::vec4(-4.40f, 2.15f, 1.80f, 1.0f);  // left front wall candle
		gubo.pointLightPositions[4] = glm::vec4(4.40f, 2.15f, 1.80f, 1.0f);   // right front wall candle
		gubo.pointLightPositions[5] = glm::vec4(0.0f, 3.85f, -2.70f, 1.0f);   // ceiling chandelier

		const glm::vec4 candleColor = glm::vec4(1.0f, 0.56f, 0.24f, 1.0f);
		gubo.pointLightColors[0] = candleColor * 1.20f;
		gubo.pointLightColors[1] = candleColor * 0.95f;
		gubo.pointLightColors[2] = candleColor * 0.95f;
		gubo.pointLightColors[3] = candleColor * 0.85f;
		gubo.pointLightColors[4] = candleColor * 0.85f;
		gubo.pointLightColors[5] = glm::vec4(1.0f, 0.66f, 0.34f, 1.0f) * 1.45f;

		gubo.materialParams = glm::vec4(0.08f, 28.0f, 1.0f, 0.0f);
		gubo.lightCounts = glm::ivec4(6, 0, 0, 0);

		DSglobal.map(currentImage, &gubo, 0);

		// defines the local parameters for the uniforms
		UniformBufferObject ubo{};		

		int instanceId;
		// character
		for(instanceId = 0; instanceId < SC.TI[0].InstanceCount; instanceId++) {
			ubo.mMat = SC.TI[0].I[instanceId].Wm;
			ubo.mvpMat = ViewPrj * ubo.mMat;
			
			// DS[1] = Pchar pass (main render): set0=DSLglobal, set1=DSLlocal
			SC.TI[0].I[instanceId].DS[0][0]->map(currentImage, &gubo, 0); // global (light/camera)
			SC.TI[0].I[instanceId].DS[0][1]->map(currentImage, &ubo, 0); // camera MVPs
		}
		
		// updates the FPS
		static float elapsedT = 0.0f;
		static int countedFrames = 0;
		
		countedFrames++;
		elapsedT += deltaT;
		if(elapsedT > 1.0f) {
			float Fps = (float)countedFrames / elapsedT;
			
			std::ostringstream oss;

			oss << "FPS: " << Fps << "\n";



			txt.print(1.0f, 1.0f, oss.str(), 1, "CO", false, false, true,TAL_RIGHT,TRH_RIGHT,TRV_BOTTOM,{1.0f,0.0f,0.0f,1.0f},{0.8f,0.8f,0.0f,1.0f});

			elapsedT = 0.0f;
		    countedFrames = 0;
		}

		updateInteractionText();
		txt.updateCommandBuffer();
	}
	
	float GameLogic() {
		// Camera FOV-y, Near Plane and Far Plane
		const float FOVy = glm::radians(45.0f);
		const float nearPlane = 0.1f;
		const float farPlane = 100.f;

		// Integration with the timers and the controllers
		float deltaT;
		glm::vec3 m = glm::vec3(0.0f), r = glm::vec3(0.0f);
		bool fire = false;
		getSixAxis(deltaT, m, r, fire);

		// Projection
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		// // View
		// // Temporary debug camera placed inside the tavern, so the entrance door
		// // does not block the fixed view before first-person movement is added.
		// View = glm::lookAt(glm::vec3(0.0f, 1.7f, 2.6f),   // Pos
		// 				   glm::vec3(0.0f, 0.9f, -1.8f),  // Target
		// 				   glm::vec3(0.0f, 1.0f, 0.0f));

		// View
		// Update camera rotation
		float pitchInput = r.x;
		// getSixAxis maps keyboard Up/Down opposite to our camera pitch convention.
		if(glfwGetKey(window, GLFW_KEY_UP) && !glfwGetKey(window, GLFW_KEY_DOWN)) {
			pitchInput = 1.0f;
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN) && !glfwGetKey(window, GLFW_KEY_UP)) {
			pitchInput = -1.0f;
		}

		yaw += rotateSpeed * r.y * deltaT;
		pitch += rotateSpeed * pitchInput * deltaT;
		pitch = glm::clamp(pitch, glm::radians(-75.0f), glm::radians(75.0f));

		// Compute camera directions
		glm::vec3 viewForward = glm::normalize(glm::vec3(
			glm::sin(yaw) * glm::cos(pitch),
			glm::sin(pitch),
			-glm::cos(yaw) * glm::cos(pitch)
		));

		glm::vec3 moveForward(
			glm::sin(yaw),
			0.0f,
			-glm::cos(yaw)
		);

		glm::vec3 right(
			glm::cos(yaw),
			0.0f,
			glm::sin(yaw)
		);

		// Move camera
		cameraPos -= moveForward * m.z * moveSpeed * deltaT;
		cameraPos += right * m.x * moveSpeed * deltaT;
		cameraPos.y += m.y * verticalSpeed * deltaT;

		if(cameraRoomLimitFlag == 1) {
			cameraPos = glm::clamp(cameraPos, cameraRoomMin, cameraRoomMax);
		}

		// NPC interaction is evaluated after camera movement and room clamping.
		const bool ePressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
		const bool eJustPressed = ePressed && !interactionKeyWasPressed;
		const bool key1Pressed = glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS ||
								 glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS;
		const bool key2Pressed = glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS ||
								 glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS;
		const bool key3Pressed = glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS ||
								 glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS;
		const bool drinkChoicePressed = key1Pressed || key2Pressed || key3Pressed;
		const bool drinkChoiceJustPressed = drinkChoicePressed && !drinkChoiceKeyWasPressed;
		const int drinkChoice = key1Pressed ? 1 : (key2Pressed ? 2 : (key3Pressed ? 3 : 0));
		const bool nearGreeter = horizontalDistance(cameraPos, greeterPos) < greeterInteractionRadius;
		const bool nearBartender = horizontalDistance(cameraPos, bartenderPos) < bartenderInteractionRadius;

		message.clear();

		if(nearGreeter) {
			if(eJustPressed) {
				if(registered) {
					registered = false;
					orderCompleted = false;
					bartenderMenuOpen = false;
					selectedDrink.clear();
					greeterFeedback = "Checkout completed. See you next time!\nPress E to register again";
				} else {
					registered = true;
					greeterFeedback = "Registration completed. You may proceed to order.\nPress E to check out";
				}
			}

			if(!greeterFeedback.empty()) {
				message = greeterFeedback;
			} else {
				message = registered ? "You're registered. Do you want to check out? \nPress E to check out"
									 : "Press E to register";
			}
		} else if(nearBartender) {
			greeterFeedback.clear();

			if(!registered) {
				message = "Hello dear customer, please register with the greeter before ordering.";
			} else if(bartenderMenuOpen) {
				if(drinkChoiceJustPressed && drinkChoice != 0) {
					selectedDrink = drinkNameFromChoice(drinkChoice);
					orderCompleted = true;
					bartenderMenuOpen = false;
					message = "You ordered " + selectedDrink + ". Enjoy!";
				} else {
					message = drinkChoiceMenuText();
				}
			} else if(orderCompleted) {
				if(eJustPressed) {
					orderCompleted = false;
					selectedDrink.clear();
					bartenderMenuOpen = true;
					message = drinkChoiceMenuText();
				} else {
					message = "You ordered " + selectedDrink + ". Press E to order again";
				}
			} else if(eJustPressed) {
				bartenderMenuOpen = true;
				message = drinkChoiceMenuText();
			} else {
				message = "Press E to order";
			}
		} else {
			greeterFeedback.clear();
		}

		if(!nearBartender) {
			orderCompleted = false;
			bartenderMenuOpen = false;
			selectedDrink.clear();
		}

		registerHintShown = nearGreeter && !registered;
		orderHintShown = nearBartender && !orderCompleted;
		interactionKeyWasPressed = ePressed;
		drinkChoiceKeyWasPressed = drinkChoicePressed;


		// Build view matrix
		View = glm::lookAt(
			cameraPos,
			cameraPos + viewForward,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		// View-Projection
		ViewPrj = Prj * View;

		return deltaT;
	}
};


// This is the main: probably you do not need to touch this!
int main() {
    DungeonTavernNPCApp app;

    try {
        app.run(false);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
