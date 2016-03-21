/* statfs.hpp
Information about the volume storing a file
(C) 2015 Niall Douglas http://www.nedprod.com/
File Created: Dec 2015


Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef BOOST_AFIO_STATFS_H
#define BOOST_AFIO_STATFS_H

#include "config.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)  // dll interface
#endif

BOOST_AFIO_V2_NAMESPACE_BEGIN

class handle;

/*! \struct statfs_t
\brief Metadata about a filing system. Unsupported entries are all bits set.
*/
struct BOOST_AFIO_DECL statfs_t
{
  struct f_flags_t
  {
    uint32_t rdonly : 1;           //!< Filing system is read only                                      (Windows, POSIX)
    uint32_t noexec : 1;           //!< Filing system cannot execute programs                           (POSIX)
    uint32_t nosuid : 1;           //!< Filing system cannot superuser                                  (POSIX)
    uint32_t acls : 1;             //!< Filing system provides ACLs                                     (Windows, POSIX)
    uint32_t xattr : 1;            //!< Filing system provides extended attributes                      (Windows, POSIX)
    uint32_t compression : 1;      //!< Filing system provides whole volume compression                 (Windows, POSIX)
    uint32_t extents : 1;          //!< Filing system provides extent based file storage (sparse files) (Windows, POSIX)
    uint32_t filecompression : 1;  //!< Filing system provides per-file selectable compression          (Windows)
  } f_flags;                       /*!< copy of mount exported flags       (Windows, POSIX) */
  uint64_t f_bsize;                /*!< fundamental filesystem block size  (Windows, POSIX) */
  uint64_t f_iosize;               /*!< optimal transfer block size        (Windows, POSIX) */
  uint64_t f_blocks;               /*!< total data blocks in filesystem    (Windows, POSIX) */
  uint64_t f_bfree;                /*!< free blocks in filesystem          (Windows, POSIX) */
  uint64_t f_bavail;               /*!< free blocks avail to non-superuser (Windows, POSIX) */
  uint64_t f_files;                /*!< total file nodes in filesystem     (POSIX) */
  uint64_t f_ffree;                /*!< free nodes avail to non-superuser  (POSIX) */
  uint32_t f_namemax;              /*!< maximum filename length            (Windows, POSIX) */
#ifndef WIN32
  int16_t f_owner; /*!< user that mounted the filesystem   (BSD, OS X) */
#endif
  uint64_t f_fsid[2];        /*!< filesystem id                      (Windows, POSIX) */
  std::string f_fstypename;  /*!< filesystem type name               (Windows, POSIX) */
  std::string f_mntfromname; /*!< mounted filesystem                 (Windows, POSIX) */
  fixme_path f_mntonname;    /*!< directory on which mounted         (Windows, POSIX) */

  //! Used to indicate what metadata should be filled in
  BOOST_AFIO_BITFIELD_BEGIN(want) { flags = 1 << 0, bsize = 1 << 1, iosize = 1 << 2, blocks = 1 << 3, bfree = 1 << 4, bavail = 1 << 5, files = 1 << 6, ffree = 1 << 7, namemax = 1 << 8, owner = 1 << 9, fsid = 1 << 10, fstypename = 1 << 11, mntfromname = 1 << 12, mntonname = 1 << 13, all = (unsigned) -1 }
  BOOST_AFIO_BITFIELD_END(want)
  //! Constructs a default initialised instance (all bits set)
  statfs_t()
  {
    size_t frontbytes = ((char *) &f_fstypename) - ((char *) this);
    memset(this, 0xff, frontbytes);
    memset(this, 0, sizeof(f_flags));
  }
  //! Constructs a filled instance, throwing as an exception any error which might occur
  statfs_t(handle &h, want wanted = want::all)
      : statfs_t()
  {
    auto v(fill(h, wanted));
    if(v.has_error())
      throw std::system_error(v.get_error());
  }
  //! Fills in the structure with metadata, returning number of items filled in
  BOOST_AFIO_HEADERS_ONLY_MEMFUNC_SPEC result<size_t> fill(handle &h, want wanted = want::all) noexcept;
};

BOOST_AFIO_V2_NAMESPACE_END

#if BOOST_AFIO_HEADERS_ONLY == 1 && !defined(DOXYGEN_SHOULD_SKIP_THIS)
#define BOOST_AFIO_INCLUDED_BY_HEADER 1
#ifdef WIN32
#include "detail/impl/windows/statfs.ipp"
#else
#include "detail/impl/posix/statfs.ipp"
#endif
#undef BOOST_AFIO_INCLUDED_BY_HEADER
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
