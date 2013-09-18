///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_STORAGE_SERVER_H
#define MYENGINE_STORAGE_SERVER_H

#include "Config.h"

#include <list>
#include <string>
#include <memory>
#include <stdexcept>

namespace MyEngine {

  // ----------------------------------------------------------------------- //

  class Archive {}; // Dummy

  // ----------------------------------------------------------------------- //

  /// <summary>Manages storage related stuff</summary>
  class StorageServer {

    /// <summary>Reads archive files like zips and rars</summary>
    public: class ArchiveReader {

      /// <summary>Releases any resources owned by the reader</summary>
      public: virtual ~ArchiveReader() {}

      /// <summary>Checks whether the reader can open an archive</summary>
      /// <param name="filename">Name of the file that will be checked</param>
      public: virtual bool canOpenArchive(const std::string &filename) = 0;

      /// <summary>Opens an archive for reading</summary>
      /// <param name="filename">Filename of the archive to open</param>
      public: virtual std::auto_ptr<Archive> openArchive(
        const std::string &filename
      ) = 0;

    };

    /// <summary>A list of archive readers</summary>
    private: typedef std::list<ArchiveReader *> ArchiveReaderList;

    /// <summary>Destroys the storage server</summary>
    public: MYENGINE_API ~StorageServer() {
      for(
        ArchiveReaderList::reverse_iterator it = this->archiveReaders.rbegin();
        it != this->archiveReaders.rend();
        ++it
      ) {
        delete *it;
      }
    }

    /// <summary>Allows plugins to add new archive readers</summary>
    public: MYENGINE_API void addArchiveReader(
      std::auto_ptr<ArchiveReader> archiveReader
    ) {
      this->archiveReaders.push_back(archiveReader.release());
    }
    
    /// <summry>
    ///   Opens an archive by searching for a matching archive reader
    /// </summary>
    /// <param name="filename">File a reader will be searched for</param>
    public: MYENGINE_API std::auto_ptr<Archive> openArchive(
      const std::string &filename
    ) {
      for(
        ArchiveReaderList::iterator it = this->archiveReaders.begin();
        it != this->archiveReaders.end();
        ++it
      ) {
        if((*it)->canOpenArchive(filename))
          return (*it)->openArchive(filename);
      }

      throw std::runtime_error("Invalid or unsupported archive type");
    }

    private: ArchiveReaderList archiveReaders; ///< All available archive readers

  };

  // ----------------------------------------------------------------------- //

} // namespace MyEngine

#endif // MYENGINE_STORAGE_SERVER_H
