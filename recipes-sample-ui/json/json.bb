SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "git://github.com/nlohmann/json.git;protocol=https;branch=develop;tag=v3.10.2"
S = "${WORKDIR}/git"

do_install() {
    install -d ${D}${includedir}
    cp -r ${S}/include/nlohmann ${D}${includedir}
}
