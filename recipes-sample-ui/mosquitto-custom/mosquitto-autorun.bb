SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
# LICENSE = "MIT"
# LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
LICENSE = "CLOSED"

SRC_URI = "file://."
S = "${WORKDIR}"

inherit systemd

do_install() {
    install -d ${D}${sysconfdir}/mosquitto
    install -m 0755 ${S}/mosquitto-sample.conf ${D}${sysconfdir}/mosquitto/mosquitto-sample.conf
    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${S}/mosquitto-sample.service ${D}${systemd_unitdir}/system/mosquitto-sample.service
}

FILES:${PN} += "${sysconfdir}/mosquitto/mosquitto-sample.conf"
FILES:${PN} += "${systemd_unitdir}/system/mosquitto-sample.service"

# SYSTEMD_AUTO_ENABLE:${PN} = "enable"
SYSTEMD_SERVICE:${PN} = "mosquitto-sample.service"
