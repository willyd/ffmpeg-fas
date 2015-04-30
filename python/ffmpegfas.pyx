from libc.stdlib cimport malloc, free
from libc.string cimport memset

# Import the Python-level symbols of numpy
import numpy as np
 
# Import the C-level symbols of numpy
cimport numpy as np

# Numpy must be initialized. When using numpy from C or Cython you must
# _always_ do that, or you will have segfaults
np.import_array()
 
 
# Import the exposed function from ffmpeg_fas.h
cimport cffmpegfas
cffmpegfas.fas_initialize(cffmpegfas.FAS_FALSE, cffmpegfas.FAS_BGR24)


cdef class VideoReader:
    cdef cffmpegfas.fas_context_ref_type m_ctx;

    def __cinit__(self):
        self.m_ctx = NULL
    
    def __dealloc__(self):
        self.close()
            
    def open(self, filename):     
        cdef cffmpegfas.fas_error_type res
        res = cffmpegfas.fas_open_video(&self.m_ctx, filename)
        if res != cffmpegfas.FAS_SUCCESS:
            raise ValueError("Error opening video file")
        return True
        
    def close(self):
        if self.is_open:
            cffmpegfas.fas_close_video(self.m_ctx)
    
    property is_open: 
        def __get__(self):
            return self.m_ctx != NULL
                
    property frame_count:
        def __get__(self):
            if self.is_open:
                return cffmpegfas.fas_get_frame_count(self.m_ctx)
            return 0
        
    property fps:
        def __get__(self):
            cdef int num
            cdef int den
            if self.is_open:        
                cffmpegfas.fas_get_fps(self.m_ctx, &num, &den)
                return float(num) / den
            return 0.0
        
    property current_frame_index:
        def __get__(self):
            if self.is_open:        
                return cffmpegfas.fas_get_frame_index(self.m_ctx)            
            return -1
        
    
        def __set__(self, index):
            cdef cffmpegfas.fas_error_type res
            if self.is_open:   
                if self.current_frame_index != index:
                    if self.current_frame_index+1 == index: 
                        res = cffmpegfas.fas_step_forward(self.m_ctx)                   
                    else:
                        res = cffmpegfas.fas_seek_to_frame(self.m_ctx, index)
                        
                    if res != cffmpegfas.FAS_SUCCESS:
                        raise ValueError("Error seeking to frame %d" %index)
        
    
    property width:
        def __get__(self):
            if self.is_open:
                return cffmpegfas.fas_get_current_width(self.m_ctx)
            return -1
        
    property height:
        def __get__(self):
            if self.is_open:
                return cffmpegfas.fas_get_current_height(self.m_ctx)
            return -1
        
    def get_frame(self, index):
        cdef cffmpegfas.fas_error_type res
        cdef cffmpegfas.fas_raw_image_type fas_img
        cdef np.npy_intp shape[3]
        self.current_frame_index = index
        res = cffmpegfas.fas_get_frame(self.m_ctx, &fas_img)
        if res != cffmpegfas.FAS_SUCCESS:
            raise ValueError("failed to read frame %d" %index)
        
        shape[0] = fas_img.height
        shape[1] = fas_img.width
        shape[2] = fas_img.bytes_per_line / fas_img.width      
                        
        ndarray = np.PyArray_SimpleNewFromData(3, shape, np.NPY_UINT8, fas_img.data)
        ndarray = ndarray.copy()
        cffmpegfas.fas_free_frame(fas_img);
        return ndarray
        
    def __len__(self):
        return self.frame_count
        
    def __getitem__(self, index):
        return self.get_frame(index)
        
        
           
# cdef class AVIFile:
    
    # cdef cvfw.PAVIFILE pavi
    # cdef cvfw.LPAVIFILEINFO paviinfo    
    # cdef cvfw.PAVISTREAM pstream
    # cdef cvfw.PGETFRAME pframe
    # cdef cvfw.LPBITMAPINFOHEADER bmpinfo
    # cdef cvfw.BITMAPINFOHEADER bi
    
    # def __cleanup(self):
        # if self.pframe is not NULL:
            # cvfw.AVIStreamGetFrameClose(self.pframe)
            # self.pframe = NULL 
        # if self.pstream is not NULL:
            # cvfw.AVIStreamRelease(self.pstream)
            # self.pstream = NULL    
        # memset(self.paviinfo,0,sizeof(cvfw.AVIFILEINFO))
        # if self.pavi is not NULL:
            # cvfw.AVIFileRelease(self.pavi)
            # self.pavi = NULL
  
    # def __cinit__(self):
        # self.pavi     = NULL
        # self.paviinfo = alloc_aviinfo()       
        # self.pstream  = NULL
        # self.pframe   = NULL   
        # memset(&self.bi, 0, sizeof(self.bi))
        # self.bmpinfo  = <cvfw.LPBITMAPINFOHEADER>cvfw.AVIGETFRAMEF_BESTDISPLAYFMT
        # cvfw.AVIFileInit()
        
    # def __dealloc__(self):
        # self.__cleanup()
        # free_aviinfo(self.paviinfo)
        # self.paviinfo = NULL
        # cvfw.AVIFileExit()
        
    # def open(self, filename, mode):         
        # cdef cvfw.LPCLSID pguid = NULL  
        # # open the avi file
        # ret = cvfw.AVIFileOpen(&self.pavi,filename,mode,pguid)    
        # if ret != 0:
            # self.__cleanup()
            # raise ValueError("Could not open AVI file")    
        # ret = cvfw.AVIFileInfo(self.pavi,self.paviinfo,sizeof(cvfw.AVIFILEINFO))
        # if ret != 0:
            # self.__cleanup()
            # raise ValueError("Could not get AVI file info")
        # # get the first video stream      
        # ret = cvfw.AVIFileGetStream(self.pavi, &self.pstream, cvfw.streamtypeVIDEO, 0)
        # if ret != 0:
            # self.__cleanup()
            # raise ValueError("Could not get video stream")
        # # open the stream frame            
        # self.bi.biSize = sizeof(self.bi)
        # self.bi.biWidth = self.width()
        # self.bi.biHeight = self.height()        
        # self.bi.biPlanes = 1        
        # self.bi.biBitCount = 24
        # self.bi.biCompression = cvfw.BI_RGB
        # self.bi.biSizeImage = self.bi.biWidth * self.bi.biHeight * self.bi.biBitCount / 8;        
        
        # self.pframe = cvfw.AVIStreamGetFrameOpen(self.pstream, &self.bi)
        # if self.pframe is NULL:
            # self.bmpinfo = NULL
            # self.__cleanup()
            # raise ValueError("Could not get stream frame")
        # self.bmpinfo = &self.bi
    
    # def close(self):
        # self.__cleanup()    
        
    # def frame_count(self):
        # if self.pstream is not NULL:
            # return int(cvfw.AVIStreamLength(self.pstream))
        # return 0
        
    # def get_frame(self, frame):
        
        # cdef cvfw.LPBITMAPINFOHEADER bh
        # cdef cvfw.BYTE* dib
        # cdef cvfw.BYTE* data
        # cdef int palettesize
        # cdef int bpp
        # cdef np.npy_intp shape[3]
                      
        # if self.pframe is not NULL:
            # dib = <cvfw.BYTE*>cvfw.AVIStreamGetFrame(self.pframe, frame)
            # bh = <cvfw.LPBITMAPINFOHEADER>dib
            # palettesize = 0
            # bpp = bh.biBitCount
            # if bpp == 8:
                # palettesize = bh.biClrUsed * sizeof(cvfw.RGBQUAD)
            
            # data = dib + sizeof(cvfw.BITMAPINFOHEADER) + palettesize;
                    
            # shape[0] = self.height()
            # shape[1] = self.width()
            # shape[2] = bpp / 8           
                        
            # ndarray = np.PyArray_SimpleNewFromData(3, shape, np.NPY_UINT8, data)
            
            # # make sure we don't delete this instance before we are done with the
            # # array
            # return np.flipud(ndarray).copy()
        # return None
                                                                    
    # def width(self):
        # return self.paviinfo.dwWidth        
        
    # def height(self):
        # return self.paviinfo.dwHeight     

    # def read_data(self,fourcc,dtype):
        # cdef cvfw.DWORD ckid
        # cdef cvfw.LONG length
        # cdef char* data
        
        # ckid = make_fourcc(fourcc)
        
        # cvfw.AVIFileReadData(self.pavi,ckid,NULL, &length)
        
        # dt = np.dtype(dtype)
        # size = length / dt.itemsize
        
        # data_array = np.zeros((size),dtype=dtype)
        
        # data = np.PyArray_BYTES(data_array)
                       
        # cvfw.AVIFileReadData(self.pavi,ckid, data, &length)
        
        # return data_array
        
        
        
            
       
        