#-------------------------------------------------
#
# Project created by QtCreator 2019-05-10T15:04:03
#
#-------------------------------------------------

QT       += widgets multimedia thelib
CONFIG   += c++14

TARGET = tttermwidget
TEMPLATE = lib

DEFINES += TTTERMWIDGET_LIBRARY

system("pkg-config --version") {
    CONFIG += link_pkgconfig
    system("pkg-config --exists libutf8proc") {
        message("Building with Utf8Proc support");
        PKGCONFIG += libutf8proc
        DEFINES += HAVE_UTF8PROC
    } else {
        message("Utf8Proc not found on this system.");
    }
}

macx {
    KB_LAYOUT_DIR = /Contents/Frameworks/tttermwidget.framework/Resources/kb-layouts
    COLORSCHEMES_DIR = /Contents/Frameworks/tttermwidget.framework/Resources/color-schemes
    TRANSLATIONS_DIR = /Contents/Frameworks/tttermwidget.framework/Resources/translations

    DEFINES += KB_LAYOUT_DIR="\\\"$$KB_LAYOUT_DIR\\\""
    DEFINES += COLORSCHEMES_DIR="\\\"$$COLORSCHEMES_DIR\\\""
    DEFINES += TRANSLATIONS_DIR="\\\"$$TRANSLATIONS_DIR\\\""
    DEFINES += HAVE_UTMPX UTMPX_COMPAT
}

unix:!macx {
    KB_LAYOUT_DIR = /usr/share/tttermwidget/kb-layouts
    COLORSCHEMES_DIR = /usr/share/tttermwidget/color-schemes
    TRANSLATIONS_DIR = /usr/share/tttermwidget/translations
    
    DEFINES += KB_LAYOUT_DIR="\\\"$$KB_LAYOUT_DIR\\\""
    DEFINES += COLORSCHEMES_DIR="\\\"$$COLORSCHEMES_DIR\\\""
    DEFINES += TRANSLATIONS_DIR="\\\"$$TRANSLATIONS_DIR\\\""
}

DEFINES += HAVE_POSIX_OPENPT HAVE_SYS_TIME_H QT_NO_FOREACH

FORMS += \
    lib/SearchBar.ui

HEADERS += \
    lib/Emulation.h \
    lib/Filter.h \
    lib/HistorySearch.h \
    lib/Pty.h \
    lib/ScreenWindow.h \
    lib/SearchBar.h \
    lib/Session.h \
    lib/TerminalDisplay.h \
    lib/Vt102Emulation.h \
    lib/kprocess.h \
    lib/kptydevice.h \
    lib/kptyprocess.h \
    lib/tttermwidget.h

SOURCES += \
    lib/BlockArray.cpp \
    lib/ColorScheme.cpp \
    lib/Emulation.cpp \
    lib/Filter.cpp \
    lib/History.cpp \
    lib/HistorySearch.cpp \
    lib/KeyboardTranslator.cpp \
    lib/Pty.cpp \
    lib/Screen.cpp \
    lib/ScreenWindow.cpp \
    lib/SearchBar.cpp \
    lib/Session.cpp \
    lib/ShellCommand.cpp \
    lib/TerminalCharacterDecoder.cpp \
    lib/TerminalDisplay.cpp \
    lib/Vt102Emulation.cpp \
    lib/konsole_wcwidth.cpp \
    lib/kprocess.cpp \
    lib/kpty.cpp \
    lib/kptydevice.cpp \
    lib/kptyprocess.cpp \
    lib/tools.cpp \
    lib/tttermwidget.cpp

TRANSLATIONS += \
    translations/au_AU.ts \
    translations/ab_GE.ts \
    translations/aa_DJ.ts \
    translations/aa_ER.ts \
    translations/aa_ET.ts \
    translations/af_ZA.ts \
    translations/sq_AL.ts \
    translations/am_ET.ts \
    translations/ar_DZ.ts \
    translations/ar_BH.ts \
    translations/ar_TD.ts \
    translations/ar_KM.ts \
    translations/ar_DJ.ts \
    translations/ar_EG.ts \
    translations/ar_ER.ts \
    translations/ar_IQ.ts \
    translations/ar_JO.ts \
    translations/ar_KW.ts \
    translations/ar_LB.ts \
    translations/ar_LY.ts \
    translations/ar_MR.ts \
    translations/ar_MA.ts \
    translations/ar_OM.ts \
    translations/ar_PS.ts \
    translations/ar_QA.ts \
    translations/ar_SA.ts \
    translations/ar_SO.ts \
    translations/ar_SD.ts \
    translations/ar_SY.ts \
    translations/ar_TZ.ts \
    translations/ar_TN.ts \
    translations/ar_AE.ts \
    translations/ar_YE.ts \
    translations/hy_AM.ts \
    translations/as_IN.ts \
    translations/az_AZ.ts \
    translations/be_BY.ts \
    translations/bn_BD.ts \
    translations/bi_VU.ts \
    translations/bs_BA.ts \
    translations/bg_BG.ts \
    translations/my_MM.ts \
    translations/ca_AD.ts \
    translations/zh_CN.ts \
    translations/zh_SG.ts \
    translations/cr_CA.ts \
    translations/hr_HR.ts \
    translations/cs_CZ.ts \
    translations/da_DK.ts \
    translations/dv_MV.ts \
    translations/nl_NL.ts \
    translations/nl_BE.ts \
    translations/dz_BT.ts \
    translations/en_AU.ts \
    translations/en_US.ts \
    translations/en_GB.ts \
    translations/en_NZ.ts \
    translations/en_CA.ts \
    translations/eo.ts    \
    translations/et_EE.ts \
    translations/fj_FJ.ts \
    translations/fi_FI.ts \
    translations/fr_CA.ts \
    translations/fr_FR.ts \
    translations/ka_GE.ts \
    translations/de_DE.ts \
    translations/de_AT.ts \
    translations/de_CH.ts \
    translations/el_GR.ts \
    translations/gn_PY.ts \
    translations/ht_HT.ts \
    translations/ha_NE.ts \
    translations/he_IL.ts \
    translations/hi_IN.ts \
    translations/ho_PG.ts \
    translations/hu_HU.ts \
    translations/id_ID.ts \
    translations/ga_IE.ts \
    translations/ig_NG.ts \
    translations/is_IS.ts \
    translations/it_IT.ts \
    translations/iu_CA.ts \
    translations/ja_JP.ts \
    translations/jv_IN.ts \
    translations/kk_KZ.ts \
    translations/km_KH.ts \
    translations/ki_KE.ts \
    translations/rw_RW.ts \
    translations/ky_KG.ts \
    translations/ko_KR.ts \
    translations/ku_IR.ts \
    translations/kj_AO.ts \
    translations/kj_NA.ts \
    translations/lb_LU.ts \
    translations/lg_UG.ts \
    translations/ln_CD.ts \
    translations/lo_LA.ts \
    translations/lt_LT.ts \
    translations/lv_LV.ts \
    translations/mk_MK.ts \
    translations/mg_MG.ts \
    translations/ms_MY.ts \
    translations/ms_SG.ts \
    translations/mt_MT.ts \
    translations/mi_NZ.ts \
    translations/mn_MN.ts \
    translations/ne_NP.ts \
    translations/nb_NO.ts \
    translations/nn_NO.ts \
    translations/pa_PK.ts \
    translations/pa_IN.ts \
    translations/fa_IR.ts \
    translations/pl_PL.ts \
    translations/pt_AO.ts \
    translations/pt_BR.ts \
    translations/pt_CV.ts \
    translations/pt_TL.ts \
    translations/pt_GQ.ts \
    translations/pt_GW.ts \
    translations/pt_MZ.ts \
    translations/pt_PT.ts \
    translations/pt_ST.ts \
    translations/rn_BI.ts \
    translations/ro_RO.ts \
    translations/ro_MD.ts \
    translations/ru_RU.ts \
    translations/ru_BY.ts \
    translations/ru_KZ.ts \
    translations/ru_KG.ts \
    translations/sm_WS.ts \
    translations/sg_CF.ts \
    translations/sr_RS.ts \
    translations/sr_BA.ts \
    translations/si_LK.ts \
    translations/sk_SK.ts \
    translations/sl_SI.ts \
    translations/so_SO.ts \
    translations/st_LS.ts \
    translations/es_ES.ts \
    translations/es_CL.ts \
    translations/es_AR.ts \
    translations/es_BO.ts \
    translations/es_CO.ts \
    translations/es_DO.ts \
    translations/es_EC.ts \
    translations/es_SV.ts \
    translations/es_GT.ts \
    translations/es_HN.ts \
    translations/es_MX.ts \
    translations/es_NI.ts \
    translations/es_PA.ts \
    translations/es_PY.ts \
    translations/es_PE.ts \
    translations/es_UY.ts \
    translations/es_VE.ts \
    translations/su_SD.ts \
    translations/sv_SE.ts \
    translations/th_TH.ts \
    translations/tk_TM.ts \
    translations/tl_PH.ts \
    translations/to_TO.ts \
    translations/tr_TR.ts \
    translations/uk_UA.ts \
    translations/ur_PK.ts \
    translations/uz_UZ.ts \
    translations/ve_ZA.ts \
    translations/vi_VN.ts \
    translations/cy_GB.ts \
    translations/zu_ZA.ts


qtPrepareTool(LUPDATE, lupdate)
genlang.commands = "$$LUPDATE -no-obsolete -source-language en_US $$_PRO_FILE_"

qtPrepareTool(LRELEASE, lrelease)
rellang.commands = "$$LRELEASE -removeidentical $$_PRO_FILE_"
QMAKE_EXTRA_TARGETS = genlang rellang
PRE_TARGETDEPS = genlang rellang

unix:!macx {
    target.path = /usr/lib

    translations.path = $$TRANSLATIONS_DIR
    translations.files = translations/*

    colorschemes.path = $$COLORSCHEMES_DIR
    colorschemes.files = lib/color-schemes/*

    kblayouts.path = $$KB_LAYOUT_DIR
    kblayouts.files = lib/kb-layouts/*

    header.path = /usr/include/tttermwidget
    header.files = lib/tttermwidget.h lib/Emulation.h lib/Filter.h

    module.files = qt_tttermwidget.pri
    module.path = $$[QMAKE_MKSPECS]/modules

    INSTALLS = target translations colorschemes kblayouts header module
}

macx {
    CONFIG += lib_bundle

    header.files = lib/tttermwidget.h lib/Emulation.h lib/Filter.h
    header.path = Headers/tttermwidget

    translations.path = Resources
    translations.files = translations

    colorschemes.path = Resources
    colorschemes.files = lib/color-schemes

    kblayouts.path = Resources
    kblayouts.files = lib/kb-layouts

    QMAKE_BUNDLE_DATA = header translations colorschemes kblayouts
}

DISTFILES += \
    qt_tttermwidget.pri
