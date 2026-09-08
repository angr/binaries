/* an x87 long double stack local (fstpt/fldt) narrowed to float */
float g_f;
float ld_local_to_f32(long double x) { long double t = x * 3.0L; float y = (float)t; g_f = y; return y; }
