add_library(VtssPHY SHARED malibu.cxx venice.cxx base.cxx)

set_target_properties(VtssPHY PROPERTIES PUBLIC_HEADER vtss_macsec_emu_base.h)
set_target_properties(VtssPHY PROPERTIES FRAMEWORK TRUE)

target_link_libraries(VtssPHY vtss_api)

install(TARGETS       VtssPHY
        LIBRARY       DESTINATION "lib"
        PUBLIC_HEADER DESTINATION "include/vtss_api")
