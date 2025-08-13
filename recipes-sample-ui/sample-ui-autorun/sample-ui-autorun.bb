SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://."
S = "${WORKDIR}"

inherit systemd

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/run-sample-ui.sh ${D}${bindir}/run-sample-ui.sh
    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${S}/sample-ui.service ${D}${systemd_unitdir}/system/sample-ui.service
}

FILES:${PN} += "${bindir}/run-sample-ui.sh"
FILES:${PN} += "${systemd_unitdir}/system/sample-ui.service"

# SYSTEMD_AUTO_ENABLE:${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "sample-ui.service"
