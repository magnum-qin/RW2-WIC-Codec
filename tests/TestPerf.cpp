#include <libraw/libraw.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

void TestDecode(int qual, const char* name, std::vector<char>& buffer) {
    LibRaw raw;
    raw.open_buffer(buffer.data(), buffer.size());
    raw.unpack();
    raw.imgdata.params.use_camera_wb = 1;
    raw.imgdata.params.output_color = 1;
    raw.imgdata.params.output_bps = 8;
    raw.imgdata.params.user_qual = qual;
    
    auto start = std::chrono::high_resolution_clock::now();
    raw.dcraw_process();
    int ret;
    libraw_processed_image_t* image = raw.dcraw_make_mem_image(&ret);
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start;
    std::cout << name << " (Qual=" << qual << ") time: " << diff.count() << " s" << std::endl;
    LibRaw::dcraw_clear_mem(image);
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    std::ifstream file(argv[1], std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    TestDecode(0, "Linear", buffer);
    TestDecode(1, "VNG", buffer);
    TestDecode(2, "PPG", buffer);
    TestDecode(3, "AHD", buffer);

    return 0;
}
