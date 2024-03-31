// ccid_config.h

#ifndef CCID_CONFIG_H
#define CCID_CONFIG_H

// Memory Constraints: Uncomment ONE option based on your memory availability 
//#define CCID_MEMORY_LIMITED  // For devices with limited RAM
#define CCID_MEMORY_ABUNDANT // For devices with ample memory

// Protocol Support: Uncomment the protocols your CCID supports
#define CCID_SUPPORTS_TPDU_T0 
#define CCID_SUPPORTS_TPDU_T1
// Escape Command Settings
#define MAX_ESCAPE_DATA_SIZE 256 // Example value, adjust as needed

//#define CCID_SUPPORTS_SHORT_APDU
//#define CCID_SUPPORTS_EXTENDED_APDU

// Automatically calculate MAX_DATA_SIZE based on settings
#if defined(CCID_MEMORY_LIMITED) && defined(CCID_SUPPORTS_TPDU_T0)
    #define MAX_DATA_SIZE 255 
#elif defined(CCID_SUPPORTS_EXTENDED_APDU)
    #define MAX_DATA_SIZE 65544
#else // A reasonable default if nothing else is defined
    #define MAX_DATA_SIZE 1024 
#endif 

#endif // CCID_CONFIG_H
