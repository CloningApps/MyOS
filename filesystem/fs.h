#ifndef FS_H
#define FS_H

void init_filesystem();
void read_file(const char* filename);
void write_file(const char* filename, const char* content);

#endif

