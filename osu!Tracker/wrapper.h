#ifdef __cplusplus
extern "C" {
#endif
	void init();
	void OpenLink(const char* link);
	const char* gradientText(const char* text, const char* color_start, const char* color_end);
	const char* solidText(const char* text, const char* color);
	
#ifdef __cplusplus
}
#endif