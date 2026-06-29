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

## Shader + Pipeline

**规则要求的重点 PDF 明确要求项目代码里能展示并解释这些部分：**

- [x] 自己写的 shaders。
- [x] vertex format：位置、normal、UV，必要时 tangent。
- [x] uniform blocks / descriptor layouts。
- [x] graphics pipelines 和 render passes。
- [x] command buffer 中如何记录 draw calls。
- [x] shader 如何用光照模型决定最终像素颜色。
- [x] fragment shader 应该包含直接光照和间接光照近似，例如 ambient。
- [x] shader 参数应通过 uniform / push constants 控制，例如灯光位置、颜色、Phong/Blinn 指数、viewer position、材质参数等。
- [ ] 可以有多个 pipeline，每个 pipeline 有自己的 shader 和 vertex format / descriptor set layout。

## Dev Process:

- 增加R/F的Z轴视角平移，设置了移动上下限为天花板和地板。

- Camera: set a presentation variable, if equal to 1 then cannot exceed the room boundaries. If 0 then free movements

- 门、墙体的灰色背板和石砖墙面之间有距离。用AI生成了干净的obj纹理，首先确定了房间整体尺寸，以及物体摆放，确定室内陈设的绝对坐标后，再切换为素材资源对墙体位置进行微调。

  针对内外墙的问题，现在限制相机不能跨出房间范围。

- XYZ位置已微调并锁定：

  - 吧台大桌子`bar_counter_table`、四角四个垫高的小箱子`bar_support_box_fl/fr/bl/br`
  - 吧台左边瓶架子`back_cupboard_L`
  - 吧台右边抽屉柜`back_locker_R`
  - 左侧红酒桶堆`left_barrel_stack`
  - 左侧红酒圆桌`left_wine_table`
  - 左侧门口红酒桶`front_barrel_L`
  - 左侧圆桌`left_round_table` + 三个凳子`left_table_chair_a/b/c`
  - 右侧长桌`right_bench_table` + 两个凳子`right_long_table_chair_a/b`
  - 门口桌子`front_porch_table` + 一个凳子`right_porch_table_chair`
  - 两个交互NPC任务位置：`npc_bartender`、`npc_door_greeter`
  - 五个墙灯：`wall_candle_backwall/FrontLeft/FrontRight/RearLeft/RearRight`
  - 四面墙、屋顶

- NPC交互：

  - 初版能触发交互，但：
    1. 靠近触发，但远离无法消失交互
    2. 文字显示集成在fps里，显示效果不稳定，刷新率慢（1Hz）
    3. 交互触发键 `E` 没有考虑反复触发的情况
  - 改进：
    1. E 现在用 `interactionKeyWasPressed` 做边沿触发，按住不会每帧重复触发。
    2. 交互文字刷新已从 FPS 的 1 秒刷新块中拆出来，改为每帧检查 message 是否变化。
    3. 远离 NPC 后 message 会清空，并调用 `TextMaker::removeText(3)` 移除旧文字，不会残留在左上角。

**陈设：**

- [x] 加上torture素材，凸出“地牢”
- [x] 屋内其他陈设继续优化

**NPC交互：**

- [x] Bartender的距离检测范围提高，现在已经突脸了
- [x] Bartender增加一些可选项
- [x] Greeter增加checkout选项

**Shader+Pipeline：**

- [ ] bala

- [ ] 