part1: part1.cpp
	@g++ part1.cpp
	@./a.out inputs_ppm/sample_pp3_5mb.ppm output_part1.ppm
part2_1a: part2_1a.cpp
	@g++ part2_1a.cpp
	@./a.out inputs_ppm/sample_pp3_5mb.ppm output_part2_1a.ppm
part2_1b: part2_1b.cpp
	@g++ part2_1b.cpp
	@./a.out inputs_ppm/sample_pp3_5mb.ppm output_part2_1b.ppm
part2_3: part2_3.cpp
	@g++ part2_3.cpp
	@./a.out inputs_ppm/sample_pp3_5mb.ppm output_part2_3.ppm
clean:
	@rm -rf a.out a.exe