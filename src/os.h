#ifdef _WIN32
#define LINE_TERMINATOR "\r\n"
#endif


#ifdef __APPLE__
#define LINE_TERMINATOR "\r"
#endif

#if __linux__ | __unix__ | __FreeBSD__
#define LINE_TERMINATOR "\n"
#endif