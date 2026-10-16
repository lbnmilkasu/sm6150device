# sm6150-common on Android 16 / LineageOS 23.2

This branch is an experimental source port; see
[the a70q bring-up record](../a70q/README.lineage-23.2.md) and its validation report.
It is not verified on any device. The original `lineage-22.2` branch is preserved.

`common.mk` holds Samsung/Qualcomm hardware packages, proprietary integration,
permissions and init configuration. `lineage.mk` holds Lineage product packages
and overlays and is inherited by `lineage_a70q.mk`. A70 dimensions, fingerprint
position, model and board assertions remain in the a70q tree.

The following remain platform-specific integration points for other ROMs:

| Component | Requirement / adaptation |
| --- | --- |
| `lineage.mk` | Lineage SDK, Doze, charging control, AIDL LiveDisplay and touch clients |
| `livedisplay/`, `touch/` | `vendor.lineage.*-V1-ndk`, framework matrix and `hal_lineage_*` policy; port the clients/interfaces/policy or replace the service implementation |
| `BoardConfigCommon.mk` | Lineage kernel build rules, `TARGET_INIT_VENDOR_LIB`, SurfaceFlinger UDFPS extension and recovery updater hooks |
| `sepolicy/` | Samsung, Qualcomm and Lineage policy types/macros; map these to the destination ROM, preserving enforcement |
| `extract-files.py` | Lineage Python extract-utils and `hardware/lineage/compat` ABI libraries |
| `hardware/qcom-caf/*` and Samsung HALs | Matching branches, Soong namespaces, typed camera/vibrator options, crypto and RIL interfaces |

These are explicit dependencies: `common.mk` is not advertised as a standalone
AOSP tree. If the destination does not provide Lineage AIDL interfaces, merely
omitting `lineage.mk` is insufficient: Soong still parses the modules and their
interface dependencies. Supply those APIs or adapt the modules and policy too.

The AIDL migration retains single tap (`id=0`, keycode `0x1c7`) and panel Natural /
Vivid modes (`2` / `4`), ALPM commands (`2` / `0`) and sunlight brightness (`365`).
Their behavior still needs device verification. Init runs the services as system;
AIDL calls use `/dev/binder` and the platform's service-manager policy. The
single joining binder thread serializes mutable sysfs state.

Do not copy Android 16 launch-device requirements onto this P-launch device.
Keep the stock partition layout, 4 KiB kernel pages and both ARM ABIs. Do not
restore checkvintf stubs, global duplicate-rule exceptions, blanket ELF-check
suppression or permissive SELinux to hide failures.

The current 4.14 kernel cannot satisfy the installed Connectivity runtime checks.
A reviewed kernel/networking backport and matching ROM integration is required;
changing the reported kernel version does not implement the missing features.

The Android 16 reference review also opts the stock audio wrapper and common
utilities out of the newer speaker-layout ABI field, migrates charging-control
settings to typed Soong configuration in lineage.mk, explicitly disables UFFD GC
for the current kernel, and redirects the actual tinyxml2 consumer through a
repeatable extraction fixup. Evidence and pinned upstream commits are recorded in
`device/samsung/a70q/compatibility/reference-review.md`.
