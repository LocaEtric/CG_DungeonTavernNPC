> **Dungeon Tavern NPC**
>
> Design a 3D tavern environment in which the user can approach and interact with one or more virtual characters (e.g., innkeeper, bard, merchant). The user navigates in first person and can trigger the interaction by getting close to a character and pressing a key.

>**Project plan (unverified)**
>
>For the Dungeon Tavern NPC project, we plan to build a static 3D tavern environment using the provided .MGCG assets and potentially some custom models.For navigation, we will implement a first-person camera using the look-in-direction technique to compute the view matrix, allowing the user to move with keyboard and mouse input
>
>To implement the required NPC interaction, we will avoid complex collision systems and instead use a straightforward distance-check algorithm between the camera's position and the NPC's fixed local coordinates; when the user is within a specific threshold, pressing a key will trigger a simple dialogue or event
>
>For the graphics pipeline, we will strictly rely on the provided Vulkan Starter.hpp skeleton without modifying the core library.We will write custom vertex and fragment shaders to handle the rendering.To create the tavern atmosphere, our fragment shaders will implement both direct lighting (e.g., point lights for candles/torches) and indirect ambient lighting, utilizing standard reflection models like Lambert for diffuse and Phong or Blinn for specular highlights

---

```c++
	glm::mat4 ViewPrj;
	glm::mat4 View;


	// Camera parameters
	// Initial position inside the tavern
	glm::vec3 cameraPos = glm::vec3(0.0f, 1.7f, 2.6f);

	float yaw = 0.0f;

	const float moveSpeed = 3.0f;
	const float rotateSpeed = 2.0f;
```



```c++
		// Projection
		glm::mat4 Prj = glm::perspective(FOVy, Ar, nearPlane, farPlane);
		Prj[1][1] *= -1;

		// View
		// Update camera rotation
		yaw += rotateSpeed * r.y * deltaT;

		// Compute camera directions
		glm::vec3 forward(
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
		cameraPos -= forward * m.z * moveSpeed * deltaT;
		cameraPos += right * m.x * moveSpeed * deltaT;

		// Build view matrix
		View = glm::lookAt(
			cameraPos,
			cameraPos + forward,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		// View-Projection
		ViewPrj = Prj * View;

		return deltaT;
	}
};
```

---

## Dev Process:

- 增加R/F的Z轴视角平移，设置了移动上下限为天花板和地板。
- 门、墙体的灰色背板和石砖墙面之间有距离。用AI生成了干净的obj纹理，先确定了房间整体尺寸，之后再手动生成纹理贴图
- XYZ位置已调准：
  - 吧台大桌子`bar_counter_table`、四角四个垫高的小箱子`bar_support_box_fl/fr/bl/br`
  - 吧台左边瓶架子`back_cupboard_L`
  - 吧台右边抽屉柜`back_locker_R`
  - 左侧圆桌`left_round_table` + 三个凳子`left_table_chair_a/b/c`
  - 右侧长桌`right_bench_table` + 两个凳子`right_long_table_chair_a/b`
  - 门口桌子`front_porch_table` + 一个凳子`right_porch_table_chair`
  - 两个交互NPC任务位置：`npc_bartender`、`npc_door_greeter`



- [ ] 加上torture素材，凸出“地牢”