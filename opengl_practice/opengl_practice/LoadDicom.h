#pragma once

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmfg/fginterface.h"
#include "dcmtk/dcmfg/fgbase.h"
#include "dcmtk/dcmfg/fgplanpo.h"



class LoadDicom
{
public:

	int openDicom();
	
	//int LoadSlice(const ost::String& filepath, int frm_index, void* target, VolumeExtDataInfo* ext_info);
};

