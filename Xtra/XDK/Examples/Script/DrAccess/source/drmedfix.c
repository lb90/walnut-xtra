/*
ADOBE SYSTEMS INCORPORATED
Copyright 1994 - 2007 Adobe Macromedia Software LLC
All Rights Reserved

NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the 
terms of the Adobe license agreement accompanying it.  If you have received this file from a 
source other than Adobe, then your use, modification, or distribution of it requires the prior 
written permission of Adobe.
*/

/****************************************************************************
 *
 * If you write the mediaData handle for #moaComposite passed back by 
 * IMoaDrCastMem::GetMedia() to a file, you will find at the start of the 
 * byte stream in Director 5:
 *  - 8 bytes of internal info about the media handle that appears to be irrelevant
 *  - two 32-bit numbers about the media handle that are _not_ byte swapped; 
 *    these are related to the handle size.
 *  - a 16-bit header size that is _not_ byte swapped
 *  - a 32-bit version number that is _not_ byte-swapped
 *  - a 32-bit internal cast member type number that is _not_ byte-swapped
 *  - a 32-bit properties length number that is _not_ byte-swapped 
 * This comes to 30 bytes (0x001e), which is the header size value.
 ****************************************************************************/ 

#include "drmedfix.h"

#ifdef WINDOWS
#ifdef MOA_DEBUG
#include <stddef.h>		/* for offsetof() */
#endif

/*
 * If you write the mediaData handle for #moaComposite passed back by 
 * IMoaDrCastMem::GetMedia() to a file, you will find at the start of the 
 * byte stream in Director 5:
 *  - 8 bytes of internal info about the media handle that appears to be irrelevant
 *  - two 32-bit numbers about the media handle that are _not_ byte swapped; 
 *    these are related to the handle size.
 *  - a 16-bit header size that is _not_ byte swapped
 *  - a 32-bit version number that is _not_ byte-swapped
 *  - a 32-bit internal cast member type number that is _not_ byte-swapped
 *  - a 32-bit properties length number that is _not_ byte-swapped 
 * This comes to 30 bytes (0x001e), which is the header size value.
 */
/*
 * Set up packing to force the 30-byte alignment.
 * Would be better to define this as bytes, but the mem_XLong() and mem_XShort()
 * routines from Goodies\CodeFrag take long and short as parameters, not bytes.
 */
#pragma pack(push, 2)
typedef struct MediaDataHdr
{
	unsigned char	dontCare[8];
	long			mediaHdlInfo1;
	long			mediaHdlInfo2;
	short			hdrSize;
	long			version;
	long			cmType;
	long			propLength;
} MediaDataHdr;
#pragma pack(pop)

/* ------------------------------------------------------------------------ */
// Reverse the bytes in a long
long  mem_XLong(long x)
{
	MoaByte temp;
	union
	{
		long oneDWord;
		MoaByte fourBytes[4];
	} Moto;
	Moto.oneDWord = x;
	/* swap two end bytes */
	temp = Moto.fourBytes[3];
	Moto.fourBytes[3] = Moto.fourBytes[0];
	Moto.fourBytes[0] = temp;
	/* now swap the middle bytes */
	temp = Moto.fourBytes[2];
	Moto.fourBytes[2] = Moto.fourBytes[1];
	Moto.fourBytes[1] = temp;
	return (Moto.oneDWord);
}

/* ------------------------------------------------------------------------ */
// Swap the bytes of a short
short  mem_XShort(short x)
{
	MoaByte temp;
	union
	{
		short oneWord;
		MoaByte twoBytes[2];
	} Moto;
	Moto.oneWord = x;
	/* swap two end bytes */
	temp = Moto.twoBytes[1];
	Moto.twoBytes[1] = Moto.twoBytes[0];
	Moto.twoBytes[0] = temp;
	return (Moto.oneWord);
}
#endif /* WINDOWS */

/*
 * Flip the media header.
 * But we may fix the Director bug that Director doesn't flip.
 * Either both Director and this code need to check if already
 * flipped, or this code has to write an indicator bit, or ???
 *
 */
void DrFixMediaHeader(const char * pMediaData)
{
#ifdef WINDOWS
	MediaDataHdr *pMDHdr = (MediaDataHdr *)pMediaData;
	/* Should be 26 bytes between start of last (4-byte) field and start of first */

	MOA_ASSERT((offsetof(MediaDataHdr, propLength) == 26), "DrMedFix, sizeof(MediaDataHdr) seems wrong!\n");

	/* When you GetMedia, if the version has already been flipped, 
	 * the version field will be really big, so don't do work.
	 * 
	 * But on AttachMedia, there's no way to know if the media has been flipped or not.
	 * ??? What if we fix this in a later version??
	 */
	if (pMDHdr->version == 0x???

	pMDHdr->mediaHdlInfo1	= mem_XLong(pMDHdr->mediaHdlInfo1);
	pMDHdr->mediaHdlInfo2	= mem_XLong(pMDHdr->mediaHdlInfo2);
	pMDHdr->hdrSize			= mem_XShort(pMDHdr->hdrSize);
	pMDHdr->version			= mem_XLong(pMDHdr->version);
	pMDHdr->cmType			= mem_XLong(pMDHdr->cmType);
	pMDHdr->propLength		= mem_XLong(pMDHdr->propLength);
#endif
}
