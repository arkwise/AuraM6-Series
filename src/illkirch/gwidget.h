
#ifndef WIDGET_EXTRACTOR_H
#define WIDGET_EXTRACTOR_H

void open_and_parse_file(const char *file_path);
void extract_widgets(const char *content);
void reconstruct_widget(int widget_type, const char *args);

#endif // WIDGET_EXTRACTOR_H
