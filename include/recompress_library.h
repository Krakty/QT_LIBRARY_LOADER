#ifndef RECOMPRESS_LIBRARY_H
#define RECOMPRESS_LIBRARY_H

#include <string>

// Function to recompress the contents of ModifiedLibrary into a ZIP file
// named basename.zip and store it in tmpFolderRoot.
void Recompress_Library(const std::string& baseName);

#endif // RECOMPRESS_LIBRARY_H
