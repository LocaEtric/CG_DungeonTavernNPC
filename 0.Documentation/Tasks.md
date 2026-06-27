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