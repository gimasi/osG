/// @page page_jumpstart Jump Start
/// @tableofcontents
/// A short guide to perform environment setup
/// @section section_ide IDE
/// At the moment, the prefered IDE is Keil uVision, version 5 or newer,
/// which runs under Windows.
/// The remainder of this document is a tutorial by using the "Blinking Led"
/// example.
///
/// @section section_setup_keil Keil Setup
/// To begin the creation of a project, the simplest way is to start from an
/// existing example, as instance, from  the "Blinking Led" test, inside the
/// "tests/blinking_led" directory.
/// To open the project, double click on the project solution file (Led.uvprojx).
/// \image html  project_solution.png
/// \image latex project_solution.png
/// If required, let Keil to install required packages.
/// \image html  install_missing_packages.png
/// \image latex install_missing_packages.png
/// Additional packages or upgdating einstalled ones can be performed by using
/// the "Pack Installer" utility.
/// \image html  update_packages.png
/// \image latex update_packages.png
/// Please note that users can use the osG library only with supported BSP's
/// and boards.
/// Porting to other targets can be considered by osG developers.
///
/// @section section_blinking_led_sourcecode The Blinking Led example sourcecode
/// In the "Blinking Led" example, there are only two files of importance for osG
/// users:
/// * blinking_led.c: the example sourcecode
/// * osgConfig.h: the configuration file for the osG library
///
/// Both files are explained in the following subsections.
///
/// @subsection section_blinking_led_c Blinking Led code
/// The "Blinking Led" code is here explained.
/// First of all, configuration fiels, standard headers, and the library headers
/// are included.
/// @code
/// #include "osgConfig.h"
/// #include <string.h>
/// #include <osg-all-in-one.h>
/// @endcode
/// Please note that there are two options to include the osG library:
/// * The first chance is by using osg-all-in-one.h: this header file is related
///   to the osg-all-in-one.c file.
///   Basically, this includes all the required osG sources into a single file,
///   therefore, users must include/compile in their projects only the osg-all-in-one.c
///   file. This way of using osG has been designed as a convenience for users,
///   and it is the preferred way.
/// * The other chance is to include "osg.h". This inclusion requires users to
///   manually add all the osG ".c" implementation files to their project soution.
///   Since this leads to no actual advantage, it is not recommanded.
///
/// After the inclusions, follows the implementation of the application user code.
/// It can usually contain:
/// * Initialization of devices
/// * Setup of threads (in case of operative system usage)
/// * The actual application logic
///
/// In this simple example, only leds are initialized, after which the actual
/// application logic follows. Please note that the appliccation method entry
/// point must be a method taking no arguments and returning "void".
/// @code
/// static void applicationUserCode(void)
/// {
///     osg_Led green;
///     osg_Led blue;
///     osg_Led red;
///     osg_LedConfig ledConfig;
///     memset(&ledConfig, 0, sizeof(ledConfig));
///
///     ledConfig.ledId = OSG_LED_GREEN;
///     osg_Led_ctor(&green, &ledConfig);
///     ...
///     for (;;) { /* ntd */ }
/// }
/// @endcode
/// As quite usual, at the end the application method does not returns but goes
/// into an infinite loop.
///
/// Finally, the main is implemented.
/// It is used for two purposes:
/// * The base HW initialization
/// * Specify which is the application entry point. This is the method passed as
///   argument to the HW initialization routine, and in this example, it is the
///   previously declared "applicationUserCode()" method.
/// @code
/// int main(void)
/// {
///     const Bool res = osg_baseHw_start(applicationUserCode);
///     osg_assert(res == TRUE, "HW base initialization failed");
///
///     return 0;
/// }
/// @endcode
/// It is "osg_baseHw_start()" responsability to invoke the "applicationUserCode()"
/// method. Please note that the only chance to execute code after the "osg_baseHw_start()"
/// invokation is that the HW initialization fails, or after the completion of
/// the application execution, therefore, no actual useful code should be placed
/// there.
///
/// @subsection section_osgconfig_h osgConfig.h example
/// The "osgConfig.h" is the file used to configure the osG library.
/// More specifically, it configures:
/// * The board for which the user project is going to be compiled
/// * The devices that are used by the project
/// * The used operative system (optional)
///
/// The structure of the file is as follows.
/// First of all, the board configuration is performed.
/// THis section is standard, and the user should never change it.
/// @code
/// /// @name Board configuration
/// /// This section lists the available boards and boards familites.
/// /// Set the BOARD_FAMILY adn BOARD_NAME macro as preferred.
/// /// @{
/// #define STM_BOARD    0
/// #define NXP_BOARD    1
/// #define STM32_L4     0
/// #define NXP_MK28F    1
/// #if defined(STM32L476xx)
/// #define BOARD_FAMILY STM_BOARD
/// #define BOARD_NAME   STM32_L4
/// #elif defined(CPU_MK28FN2M0VMI15)
/// #define BOARD_FAMILY NXP_BOARD
/// #define BOARD_NAME   NXP_MK28F
/// #else
/// #define BOARD_FAMILY "UNKNOWN"
/// #define BOARD_NAME   "UNKNOWN"
/// #endif
/// /// @}
/// @endcode
/// The initial macros are a sort of "enum": they list in unique values:
/// * The board family (STM_BOARD, NXP_BOARD)
/// * The specific board name (STM32_L4, NXP_MK28F)
///
/// After which, some standard lines of code follows. These lines configure
/// the BOARD_FAMILY and BOARD_NAME macros, by automatically checking macros
/// provided by specific boards BSP's.
/// Users can use these declared macros to write board-specific code in their
/// applications. For example:
/// @code
/// #if BOARD_NAME == STM32_L4
/// // STM32 L4 specific code here
/// #elif BOARD_FAMILY == NXP_BOARD
/// // Code for all NXP boards here
/// #endif
/// @endcode
///
/// "osgConfig.h" file continues with the selection of drivers for required
/// devices. Please note that only the optional devices are listed. As instance,
/// GPIO and LED are mandatory, and therefore their are automatically included.
/// @code
/// /// @name Driver features
/// /// Each macro can hold:
/// /// - 1 to enable the feature
/// /// - 0 to disable it
/// /// @{
/// #define OSG_USE_I2C    1
/// #define OSG_USE_SPI    1
/// #define OSG_USE_TIMER  1
/// #define OSG_USE_UART   1
/// /// @}
/// @endcode
/// In this example, all the drivers are included, even if the "Blinking Led"
/// does not use them.
///
/// The usage of the optional operative system is stated by the following configuration
/// code.
/// @code
/// /// @name OS Names
/// /// List of available OS "names".
/// /// Set the OSG_OS_NAME to the name of the preferred one
/// /// @{
/// #define OSG_OS_NONE        0
/// #define OSG_OS_CMSIS_RTOS2 1
/// #define OSG_OS_NAME        OSG_OS_NONE
/// /// @}
/// @endcode
/// First of all, there is another "enum", listing all the available operative systems.
/// At the moment, the possible choises are "no operative system" (OSG_OS_NONE),
/// and CMSIS RTOS v2 (OSG_OS_CMSIS_RTOS2).
/// Users can edit the OSG_OS_NAME macro value to specify the preferred operative
/// system. In the "Blinking Led" example, no operative system is used.
///
/// In case of the usage of an operative system, the last section of the configuration
/// file must be edited, setting to 1 the features of interest.
/// @code
/// /// @name OS features
/// /// Each macro can hold:
/// /// - 1 to enable the feature
/// /// - 0 to disable it
/// /// Note: OSG_OS_USE_TIMERS is usually set to the
/// /// same value of OSG_USE_TIMERS
/// /// @{
/// #define OSG_OS_USE_THREAD 1
/// #define OSG_OS_USE_TIMER  OSG_USE_TIMER
/// /// @}
/// @endcode
/// Please note that the OSG_OS_USE_TIMER should never be edited: prefer to edit
/// OSG_USE_TIMER to specify when timers must be included.
///
