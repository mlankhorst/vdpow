static const int output_width = 1440;
static const int output_height = 900;
static const int input_width = 1440;
static const int input_height = 900;

static const VdpPictureInfoMPEG1Or2 info = {
    .forward_reference = VDP_INVALID_HANDLE,
    .backward_reference = VDP_INVALID_HANDLE,
    .slice_count = 0x44,
    .picture_structure = 3,
    .picture_coding_type = 1,
    .intra_dc_precision= 2,
    .frame_pred_frame_dct = 0,
    .concealment_motion_vectors = 0,
    .intra_vlc_format = 1,
    .alternate_scan = 1,
    .q_scale_type = 1,
    .top_field_first = 1,
    .full_pel_forward_vector = 0,
    .full_pel_backward_vector = 0,
    .f_code =  {{15, 15}, {15, 15}},
    .intra_quantizer_matrix = {8, 16, 19, 22, 26, 27, 29, 34, 16, 16, 22, 24, 27, 29, 34, 37, 19, 22, 26, 27, 29, 34, 34, 38, 22, 22, 26, 27, 29, 34, 37, 40, 22, 26, 27, 29, 32, 35, 40, 48, 26, 27, 29, 32, 35, 40, 48, 58, 26, 27, 29, 34, 38, 46, 56, 69, 27, 29, 35, 38, 46, 56, 69, 83},
    .non_intra_quantizer_matrix = {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
};
