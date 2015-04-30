
    
cdef extern from "ffmpeg_fas.h":

    ctypedef enum fas_color_space_type:    
        FAS_GRAY8    = 1
        FAS_RGB24    = 2
        FAS_BGR24    = 3
        FAS_ARGB32   = 4
        FAS_ABGR32   = 5
        FAS_YUV420P  = 6
        FAS_YUYV422  = 7
        FAS_UYVY422  = 8
        FAS_YUV422P  = 9
        FAS_YUV444P  = 10
    

    ctypedef struct fas_raw_image_type:
        unsigned char *data
        int width
        int height
        int bytes_per_line
        fas_color_space_type color_space
    

    cdef struct fas_context_struct:
        pass
        
    ctypedef fas_context_struct* fas_context_ref_type

    ctypedef enum fas_error_type:   
        FAS_SUCCESS
        FAS_FAILURE
        FAS_INVALID_ARGUMENT
        FAS_OUT_OF_MEMORY
        FAS_UNSUPPORTED_FORMAT
        FAS_UNSUPPORTED_CODEC
        FAS_NO_MORE_FRAMES
        FAS_DECODING_ERROR
        FAS_SEEK_ERROR

    ctypedef enum fas_boolean_type:    
      FAS_FALSE = 0
      FAS_TRUE  = 1
    
    void             fas_initialize (fas_boolean_type logging, fas_color_space_type format)
    void             fas_set_format (fas_color_space_type format)

    fas_error_type   fas_open_video  (fas_context_ref_type *context_ptr, const char *file_path)
    fas_error_type   fas_close_video (fas_context_ref_type context)

    char*            fas_error_message (fas_error_type error)

    fas_boolean_type fas_frame_available (fas_context_ref_type context)
    int              fas_get_frame_index (fas_context_ref_type context)
    fas_error_type   fas_step_forward    (fas_context_ref_type context)

    fas_error_type   fas_get_frame  (fas_context_ref_type context, fas_raw_image_type *image_ptr)
    void             fas_free_frame (fas_raw_image_type image)

    fas_error_type   fas_seek_to_nearest_key     (fas_context_ref_type context, int target_index)
    fas_error_type   fas_seek_to_frame           (fas_context_ref_type context, int target_index)

    int              fas_get_frame_count         (fas_context_ref_type context)
    int              fas_get_frame_count_fast    (fas_context_ref_type context)
    
    #fas_error_type   fas_put_seek_table  (fas_context_ref_type context, seek_table_type table)
    #seek_table_type  fas_get_seek_table  (fas_context_ref_type context)


    fas_error_type  fas_fill_420p_ptrs (fas_context_ref_type context, unsigned char *y, unsigned char *u, unsigned char *v)

    fas_error_type  fas_fill_gray8_ptr(fas_context_ref_type context, unsigned char *y)

    int  fas_get_current_width(fas_context_ref_type context)
    int  fas_get_current_height(fas_context_ref_type context)

    unsigned long long fas_get_frame_duration(fas_context_ref_type context)

    void fas_get_fps(fas_context_ref_type context, int* num, int* den)
   
    