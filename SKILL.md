---
name: modify-keil-directory
description: Reorganize a single-project Keil MCU workspace and its uVision group structure into a standardized layout for bare-metal or RTOS projects. Use when Codex needs to create or modify App/BSP/Common/Drivers/Middlewares/Project/Output directories, move files into that structure, update Keil project grouping or output paths, optionally copy utility templates, generate a root readme, or provide Keil output configuration guidance for one local Keil project.
---

# Modify Keil Directory

Use this skill to standardize the local folder layout and Keil project grouping of a single MCU project.

## Workflow

1. Before applying any changes, complete the required confirmations listed in `Mandatory Structured Choices`.
2. Confirm the target is a single-project Keil workspace, not a multi-project layout such as `boot` + `app` or `master` + `slave`.
3. Confirm whether the project uses RTOS.
4. Confirm whether the task is:
   - creating a new structure in an empty directory, or
   - restructuring an existing project.
5. Confirm whether to:
   - modify in place with backup, or
   - generate a new output directory.
6. If the current environment supports structured user-input tools with clickable choices, use them for these confirmations instead of free-form typing.
7. If structured choice tools are unavailable, fall back to short text-based multiple-choice prompts.
8. Read [references/layout-spec.md](references/layout-spec.md) before changing directories or project grouping.
9. Apply the standard directory layout.
10. Update Keil virtual groups and output paths if the user wants the Keil project adjusted too.
11. If the user opts into utility source files, run `scripts/copy_utilities.ps1` so files from `assets/utilities/` are copied into the target project's `Common/utilities/`. Do not rely on ad hoc manual copying when the script is available.
12. Copy `assets/README-template.md` to the target project root as `readme.md` and adapt names or examples to the actual project.
13. If the user asks for Keil configuration guidance, use screenshots in `assets/keil-config-guide/`.
14. Ask whether the user's Keil build tool has been added to system environment variables or `PATH`.
15. If the user answers `Yes`, first verify the actual resolved tool path in PowerShell before assuming the tool is usable. Start with `Get-Command UV4.exe`, and also check other likely Keil executable names such as `UV5.exe` or `uvision.exe` when needed.
16. If the user answers `No`, require the user to provide the absolute path to the build tool directory or executable before compile verification. Show an example such as `D:\ruanjian\keil5-MDK-ARM\UV4\`, but do not assume the tool is necessarily `UV4`.
17. After the Keil directory structure and project configuration changes are complete, use the resolved build tool path to run a compile verification when command-line execution is available in the environment.

## Mandatory Structured Choices

These items must be explicitly confirmed before changes are applied. If the runtime supports structured clickable choice tools, use them. Do not silently assume answers for these items.

1. Confirm that the target is a single-project structure.
   - Continue
   - Exit
2. Warn that this skill will modify both the local directory structure and the Keil project structure or configuration, then confirm whether to continue.
   - Continue
   - Exit
3. If the source project does not already contain utility files under `Common/utilities` or an equivalent utilities area, ask whether to copy utility files from `assets/utilities/`.
   - Yes
   - No
4. Ask whether to include Keil output-path and `fromelf` configuration guidance assets.
   - Yes
   - No
5. If it is not already clear from context, ask whether this is a new project layout or a modification of an existing project.
   - New project
   - Modify existing project
6. Ask whether to preserve a backup of the original project or generate a separate new output directory.
   - Keep backup / modify with backup
   - Generate new directory
7. Ask whether the Keil build tool has been added to system environment variables or `PATH`.
   - Yes
   - No
   - If the answer is `Yes`, first verify it in PowerShell with `Get-Command UV4.exe` and other likely Keil executable names as needed to confirm the resolved path.
   - If the answer is `No`, require the user to provide the absolute path to the build tool directory or executable. Show an example such as `D:\ruanjian\keil5-MDK-ARM\UV4\`, but do not assume the tool is necessarily `UV4`.

## Rules

- Modify directory structure and project configuration files, but do not change business source logic.
- Keep the root project directory name unchanged.
- Support only single-project Keil layouts.
- Treat Keil groups as virtual groups. They do not need to exactly mirror local directory names.
- Prefer structured clickable choice prompts for user decisions when the runtime supports them.
- Fall back to concise text-based choice prompts only when structured choice tools are unavailable.
- Treat the items in `Mandatory Structured Choices` as required confirmations, not optional suggestions.
- Keep top-level local directory naming consistent with the layout spec.
- Place toolchain-specific files such as `startup_xxx.s` and `.sct` under `Project/Keil-MDK/`.
- Do not rewrite official vendor driver logic under `Drivers/` unless the user explicitly asks for that separately.
- Do not assume that the Keil build tool is callable just because the user says it is on `PATH`; verify it first with PowerShell `Get-Command`.
- If the build tool is not on `PATH`, require an absolute path from the user before attempting compile verification.
- After restructuring, perform a compile verification when a valid build tool path is available and the runtime permits command-line execution.
- When verifying tool discovery, check likely Keil executable names such as `UV4.exe`, `UV5.exe`, or `uvision.exe` as appropriate instead of assuming a single filename.

## Resources

- Use `assets/templates/directory-tree-rtos.txt` and `assets/templates/directory-tree-baremetal.txt` for target local layout templates.
- Use `assets/templates/keil-groups-rtos.txt` and `assets/templates/keil-groups-baremetal.txt` for Keil group templates.
- Use `assets/README-template.md` for the generated root `readme.md`.
- Use `assets/gitignore/keil-template.gitignore` when the user wants a matching `.gitignore`.
- Use `scripts/copy_utilities.ps1` when the user opts into copying utility source files from `assets/utilities/`.
- Use `assets/keil-config-guide/` only when the user requests Keil output-path or `fromelf` setup guidance.

## References

- Read [references/layout-spec.md](references/layout-spec.md) for:
  - directory responsibilities,
  - RTOS vs bare-metal variants,
  - file placement rules for startup/system/interrupt files,
  - Keil grouping conventions,
  - output layout and `.bin` generation guidance.
- If Chinese reference material is more convenient for maintenance or review, also see:
  - [references/SKILL-CH.md](references/SKILL-CH.md)
  - [references/layout-spec-ch.md](references/layout-spec-ch.md)
