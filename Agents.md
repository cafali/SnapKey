# SnapKey v1.2.8 Agents 自动化修改规范

## 1. VAC Bypass 配置可调（config.cfg 可选项）

### 目标

* 允许**VAC绕过A/B**功能在`config.cfg`配置文件中保存其开关状态。
* 允许**两个模式下所有Sleep延迟**范围可在配置文件调整（区分A/B模式）。

### 配置文件示例（新增段落）

```ini
# VAC Bypass 配置
vac_bypass_a = 1        # 1=启用 0=关闭
vac_bypass_b = 0        # 1=启用 0=关闭

# VAC Bypass 延迟设置（单位：毫秒）
vac_a_min_delay = 15    # A模式最低 overlap 延迟
vac_a_max_delay = 35    # A模式最大 overlap 延迟

vac_b_min_delay = 5     # B模式最小释放-按下间隔
vac_b_max_delay = 15    # B模式最大释放-按下间隔
```

* **默认值**见上。
* **禁止负数、最大最小颠倒。**
* 未设置时，按默认值初始化。

---

## 2. 延迟生成随机数机制升级

### 目标

* 所有 VAC 绕过相关的延迟逻辑全部替换为 C++11 标准 `std::mt19937`（Mersenne Twister）+ `std::chrono`。
* 不允许使用 C 的 `rand()`，保证多线程/多实例下延迟分布均匀无碰撞。

### 必须实现

* 延迟取值每次都要走同一 `std::mt19937` 实例（优先全局静态），用 `std::random_device` 进行 seed。
* **调用样例：**

  ```cpp
  static std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> dist(min, max);
  int delay = dist(rng);
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
  ```

---

## 3. VAC 绕过菜单状态和图标动态切换

### 目标

* **新增一个 icon\_vac\_bypass.ico（128x128）**

  * 图案要求：

    * 大号 VAC 字母（可参照 Valve Anti-Cheat 原色），下方画一个长水平箭头（建议 **→ 或 --**> 结构，强调“穿透/绕过”意象）
    * 图标风格必须“和 icon.ico、icon\_off.ico 一致”，不可过于复杂，保证 16x16 到 128x128 兼容缩放。
* **图标切换规则：**

  * **SnapKey 启用**，且**任一 VAC bypass 功能启用**：显示 icon\_vac\_bypass.ico
  * **SnapKey 禁用**（isLocked=true）：只显示 icon\_off.ico（无论VAC选项）
  * 其它情况仍为普通 icon.ico
* 菜单栏**VAC bypass A/B 状态**同步菜单打钩与图标状态。

---

## 4. 稳定性与兼容性要求

* 任何图标读取失败时，fallback 到 icon.ico，不允许图标状态丢失或崩溃。
* 菜单状态与配置文件保持一致（修改菜单选项应自动写回 config.cfg，重启自动读取）。
* 保持原有的托盘右键菜单逻辑和热键绑定管理不变。

---

## 5. agents.md 生成自动化流程推荐

（适合 CI/CD 或脚本辅助开发环境）

1. 检查 config.cfg 是否存在上述新段落。不存在则自动补全默认项。
2. 替换所有 `Sleep(rand() % X)` 为 std::mt19937 + chrono 方案。
3. 更新 Tray 菜单栏，VAC bypass A/B 绑定菜单、菜单项与 config 状态同步。
4. 插入图标切换判定逻辑：任一绕过启用 && SnapKey 启用 → icon\_vac\_bypass.ico，否则按旧逻辑。
5. 变更或菜单开关时自动写 config.cfg。
6. 自动检测并加载 icon\_vac\_bypass.ico，找不到 fallback。
7. 强制所有配置解析后做一次合法性检查（无效配置自动回落到默认）。
8. 不影响原有 Group/Key 配置、版本信息、帮助菜单等其它业务。

---

## 6. icon\_vac\_bypass.ico 设计（AI自动化图标生成描述）

**prompt：**

```
A 128x128 Windows .ico file with large bold letters "VAC" in white or pale blue, on a dark or black background. Below the letters, a thick, clear, horizontal arrow (→ or --__>) passes underneath, visually suggesting "bypass" or "tunneling". The style should match classic tray icons—minimal, high-contrast, not cartoonish. Keep the icon readable at 16x16. 
```

---

## 7. 其它说明

* 所有自动化脚本与补丁操作**必须**严格遵循此 agents.md。
* 具体代码注释需要使用英语
