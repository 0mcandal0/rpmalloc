
#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <benchmark.h>
#include <thread.h>
#include <timer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_THREAD_COUNT 8

struct benchmark_arg {
	size_t size;
	size_t accumulator;
	uint64_t ticks;
	uint64_t mops;
	thread_arg thread_arg;
};

typedef struct benchmark_arg benchmark_arg;

static int benchmark_start;


static const size_t random_size[] = {
	18, 3032, 336, 3774, 552, 961, 662, 5727, 56986, 6923, 4714, 625, 929, 344, 104, 2021, 426, 924,
	5015, 3138, 531, 7180, 610, 58817, 42511, 500, 427, 446, 2704, 456, 3223, 2505, 4808, 7271,
	4273, 44269, 30, 1306, 845, 179, 638, 7149, 517, 130, 182, 3755, 634, 4012, 825, 3617, 566,
	697, 947, 86, 223, 373, 37119, 1669, 5821, 21544, 786, 4679, 7913, 4599, 952, 375, 5409, 1022,
	844, 625, 3591, 6190, 4626, 1008, 688, 7121, 870, 840, 60559, 7858, 349, 63402, 738, 44842,
	4067, 29253, 6047, 752, 7551, 1020, 276, 6206, 472, 568, 525, 811, 6091, 2395, 694, 414, 817,
	18, 746, 3398, 999, 5410, 869, 5787, 967, 1298, 265, 144, 442, 7031, 876, 343, 205, 619, 773,
	13619, 4185, 951, 4395, 29218, 287, 7182, 57294, 494, 1021, 593, 959, 332, 439, 202, 356, 6841,
	307, 468, 649, 4253, 367, 799, 28905, 2286, 9688, 271, 570, 731, 2538, 2210, 4595, 30907, 1868,
	19779, 624, 28446, 739, 62916, 850, 615, 928, 618, 842, 476, 435, 28579, 5720, 58787, 3093,
	4489, 67, 176, 933, 886, 8124, 951, 3673, 925, 63274, 35092, 687, 63822, 45618, 675, 4172, 1018,
	5628, 583, 805, 5274, 3850, 874, 793, 49, 909, 1563, 6067, 788, 7721, 1094, 3088, 303, 39, 363,
	1077, 698, 198, 1560, 17839, 42192, 409, 536, 100, 680, 1004, 493, 541, 57181, 3141, 776, 47992,
	5032, 5429, 1977, 141, 670, 5150, 507, 6172, 2147, 612, 983, 885, 7858, 1771, 21782, 62544,
	2381, 5564, 32731, 33145, 2373, 7639, 454, 5923, 22891, 101, 4681, 403, 145, 46147, 386, 5303,
	42194, 506, 806, 389, 58, 2045, 1002, 18343, 7195, 403, 16468, 809, 50620, 8093, 2174, 12, 2087,
	710, 4194, 892, 32292, 697, 361, 777, 287, 948, 239, 577, 483, 337, 986, 914, 42728, 975, 54663,
	445, 15372, 822, 11505, 505, 268, 791, 455, 59958, 22415, 11044, 240, 2280, 3252, 1286, 675,
	20559, 645, 90, 7732, 714, 7233, 6636, 6261, 551, 3393, 580, 3209, 416, 5206, 10, 784, 7459,
	741, 3398, 7248, 2325, 57439, 471, 6506, 238, 974, 8151, 89, 4836, 15411, 1645, 5406, 914, 634,
	174, 276, 800, 67, 804, 793, 343, 15, 40849, 3794, 39850, 383, 277, 620, 1472, 59, 657, 100,
	5129, 718, 365, 317, 5090, 877, 270, 954, 445, 7635, 4641, 1773, 21634, 690, 61100, 610, 422,
	5459, 669, 792, 786, 48688, 1323, 778, 824, 53506, 384, 452, 5460, 114, 648, 332, 391, 54126,
	121, 23, 39059, 101, 205, 1425, 6761, 122, 992, 364, 664, 545, 633, 49730, 30032, 833, 1800,
	58, 226, 3638, 895, 812, 38742, 58254, 3496, 59783, 26, 712, 881, 322, 3249, 887, 1639, 150,
	4111, 178, 361, 7631, 141, 512, 36494, 535, 752, 6627, 174, 674, 78, 16847, 45293, 4381, 396,
	655, 1001, 3995, 11797, 333, 9770, 1006, 877, 370, 8525, 2646, 1892, 1904, 104, 675, 74, 4802,
	394, 2406, 846, 916, 697, 959, 1078, 11443, 907, 470, 1023, 4547, 38, 7691, 217, 4268, 7677,
	300, 2990, 14798, 1634, 46730, 632, 4162, 916, 21, 858, 3068, 3302, 7863, 386, 861, 5526, 408,
	4049, 5335, 236, 764, 135, 3143, 3913, 6977, 6579, 690, 405, 838, 244, 7441, 790, 9487, 299,
	3228, 350, 290, 317, 7360, 117, 41, 58857, 96, 57903, 118, 872, 56890, 758, 32586, 548, 7449,
	3444, 30, 503, 1007, 257, 1489, 624, 161, 5520, 744, 32798, 537, 1999, 759, 74, 7742, 231, 3778,
	540, 52716, 3584, 306, 278, 788, 5977, 78, 751, 56, 51, 618, 874, 7698, 62883, 4022, 569, 556,
	2004, 96, 4651, 789, 7682, 3480, 762, 878, 542, 211, 291, 30, 26110, 6664, 229, 57088, 55603,
	4140, 785, 4347, 218, 57547, 702, 6861, 48764, 484, 8343, 752, 675, 982, 256, 617, 574, 518,
	560, 102, 6101, 719, 769, 7259, 144, 360, 38553, 219, 137, 7431, 348, 5595, 42205, 976, 814,
	8115, 557, 934, 649, 32608, 92, 1492, 607, 829, 752, 945, 685, 503, 17804, 697, 829, 102, 7963,
	22698, 175, 5640, 136, 6562, 618, 436, 5046, 5547, 934, 119, 951, 680, 712, 970, 1343, 92, 793,
	27968, 962, 140, 493, 4014, 2955, 55, 1011, 835, 473, 860, 50096, 5969, 540, 18, 141, 14352,
	1003, 1702, 997, 41448, 15827, 3814, 686, 249, 932, 43394, 44310, 771, 763, 50942, 993, 1649,
	460, 998, 4758, 6268, 958, 210, 4638, 846, 243, 1205, 146, 6818, 251, 271, 781, 3399, 38691,
	553
};

static const size_t num_alloc_ops[] = {
	13,	18,	12,	16,	27,
	34,	24,	24,	18,	14,
	12,	18,	33,	16,	27,
	27,	5,	12,	28,	7,
	25,	27,	7,	8,	23,
	26,	25,	13,	23,	6,
	11,	35,	28,	10,	14,
	23,	14,	21,	30,	29,
	21,	12,	6,	8,	30,
	32,	14,	33,	32,	16,
	27,	10,	26,	21,	10,
	17,	20,	30,	6,	26,
	33,	22,	35,	21,	28,
	9,	31,	15,	10,	19,
	32,	28,	25,	10,	16,
	25,	20,	19,	27,	19,
	23,	29,	14,	10,	21,
	33,	18,	35,	11,	6,
	32,	19,	28,	28,	24,
	31,	7,	31,	19,	29
};

static const size_t num_free_ops[] = {
	8,	6,	5,	23,	24,
	22,	21,	13,	18,	13,
	5,	7,	11,	10,	17,
	11,	21,	11,	11,	25,
	13,	23,	20,	14,	25,
	15,	20,	25,	20,	24,
	16,	3,	24,	14,	23,
	10,	25,	16,	18,	22,
	3,	6,	4,	4,	14,
	11,	16,	12,	12,	6,
	18,	7,	14,	21,	8,
	8,	9,	11,	14,	11,
	13,	5,	23,	14,	22,
	23,	14,	15,	6,	10,
	6,	11,	3,	4,	8,
	24,	8,	22,	25,	13,
	14,	23,	21,	5,	5,
	13,	14,	18,	13,	18,
	15,	7,	14,	7,	22,
	13,	6,	9,	23,	16
};

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#endif

static size_t
get_process_memory_usage(void) {
#ifdef _WIN32
	PROCESS_MEMORY_COUNTERS counters;
	memset(&counters, 0, sizeof(counters));
	counters.cb = sizeof(counters);
	GetProcessMemoryInfo(GetCurrentProcess(), &counters, sizeof(counters));
	return counters.WorkingSetSize;
#else
	return 0;
#endif
}

static void
allocate_fixed_size(void* argptr) {
	benchmark_arg* arg = argptr;
	void* pointers[1024];
	const size_t num_pointers = sizeof(pointers) / sizeof(pointers[0]);
	const size_t num_loops = 8192*1024;
	const size_t alignment[3] = { 0, 8, 16 };
	const size_t alloc_ops_count = (sizeof(num_alloc_ops) / sizeof(num_alloc_ops[0]));
	const size_t free_ops_count = (sizeof(num_free_ops) / sizeof(num_free_ops[0]));

	size_t alloc_idx = 0;
	size_t free_idx = 0;
	size_t iop;

	benchmark_thread_initialize();

	void* trigger = benchmark_malloc(16, arg->size);

	while (!benchmark_start)
		thread_yield();

	memset(pointers, 0, sizeof(pointers));

	arg->ticks = 0;
	arg->mops = 0;
	for (size_t iter = 0; iter < 4; ++iter) {
		size_t tick_start, ticks_elapsed;
		for (size_t iloop = 0; iloop < (iter ? num_loops : num_loops / 4); ++iloop) {
			tick_start = timer_current();

			const size_t alloc_op_count = num_alloc_ops[(iter + iloop) % alloc_ops_count];
			for (iop = 0; iop < alloc_op_count; ++iop) {
				if (pointers[alloc_idx]) {
					benchmark_free((void*)pointers[alloc_idx]);
					++arg->mops;
				}
				pointers[alloc_idx] = benchmark_malloc(alignment[iloop % 3], arg->size);
				++arg->mops;

				alloc_idx = (alloc_idx + 1) % num_pointers;
			}

			const size_t free_op_count = num_free_ops[(iter + iloop) % free_ops_count];
			for (iop = 0; iop < free_op_count; ++iop) {
				if (pointers[free_idx]) {
					benchmark_free((void*)pointers[free_idx]);
					++arg->mops;
					pointers[free_idx] = 0;
				}

				free_idx = (free_idx + 1) % num_pointers;
			}

			ticks_elapsed = timer_current() - tick_start;
			if (iter)
				arg->ticks += ticks_elapsed;
		}

		tick_start = timer_current();
		for (size_t iptr = 0; iptr < num_pointers; ++iptr) {
			if (pointers[iptr]) {
				benchmark_free((void*)pointers[iptr]);
				++arg->mops;
				pointers[iptr] = 0;
			}
		}
		ticks_elapsed = timer_current() - tick_start;
		if (iter)
			arg->ticks += ticks_elapsed;

		printf(".");
		fflush(stdout);
		if (iter) {
			printf(" %.2f ", timer_ticks_to_seconds(arg->ticks));
			fflush(stdout);
			if (timer_ticks_to_seconds(arg->ticks) > 300)
				break;
		}
	}

	benchmark_free(trigger);

	benchmark_thread_finalize();

	arg->accumulator += arg->mops;
}

static void
allocate_random_size(void* argptr) {
	benchmark_arg* arg = argptr;
	void** pointers;
	const size_t num_pointers = 8192*4;
	const size_t num_loops = 8192*1024;
	const size_t random_size_count = (sizeof(random_size) / sizeof(random_size[0]));
	const size_t alloc_ops_count = (sizeof(num_alloc_ops) / sizeof(num_alloc_ops[0]));
	const size_t free_ops_count = (sizeof(num_free_ops) / sizeof(num_free_ops[0]));
	const size_t alignment[3] = { 0, 8, 16 };

	size_t alloc_idx = 0;
	size_t free_idx = 0;
	size_t iop;

	benchmark_thread_initialize();

	size_t pointers_size = sizeof(void*) * num_pointers;
	pointers = benchmark_malloc(16, pointers_size);
	memset(pointers, 0, pointers_size);

	while (!benchmark_start)
		thread_yield();

	arg->ticks = 0;
	arg->mops = 0;
	for (size_t iter = 0; iter < 4; ++iter) {
		size_t tick_start, ticks_elapsed;
		for (size_t iloop = 0; iloop < num_loops; ++iloop) {
			size_t size_index = (iter * 3 + iloop * 7) % random_size_count;
			size_t size = random_size[size_index];
			if (arg->size)
				size = size % arg->size;

			tick_start = timer_current();

			const size_t alloc_op_count = num_alloc_ops[(iter + iloop) % alloc_ops_count];
			for (iop = 0; iop < alloc_op_count; ++iop) {
				if (pointers[alloc_idx]) {
					benchmark_free((void*)pointers[alloc_idx]);
					++arg->mops;
				}
				pointers[alloc_idx] = benchmark_malloc(alignment[size_index % 3], size);
				++arg->mops;

				alloc_idx = (alloc_idx + 1) % num_pointers;
			}

			const size_t free_op_count = num_free_ops[(iter + iloop) % free_ops_count];
			for (iop = 0; iop < free_op_count; ++iop) {
				if (pointers[free_idx]) {
					benchmark_free((void*)pointers[free_idx]);
					++arg->mops;
					pointers[free_idx] = 0;
				}

				free_idx = (free_idx + 1) % num_pointers;
			}

			ticks_elapsed = timer_current() - tick_start;
			if (iter)
				arg->ticks += ticks_elapsed;
		}

		tick_start = timer_current();
		for (size_t iptr = 0; iptr < num_pointers; ++iptr) {
			if (pointers[iptr]) {
				benchmark_free((void*)pointers[iptr]);
				++arg->mops;
				pointers[iptr] = 0;
			}
		}
		ticks_elapsed = timer_current() - tick_start;
		if (iter)
			arg->ticks += ticks_elapsed;

		printf(".");
		fflush(stdout);
		if (iter) {
			printf(" %.2f ", timer_ticks_to_seconds(arg->ticks));
			fflush(stdout);
			if (timer_ticks_to_seconds(arg->ticks) > 300)
				break;
		}
	}

	benchmark_free(pointers);

	benchmark_thread_finalize();

	arg->accumulator += arg->mops;
}

int main(int argc, char** argv) {
	if (timer_initialize() < 0)
		return -1;
	if (benchmark_initialize() < 0)
		return -2;

	(void)sizeof(argc);
	(void)sizeof(argv);

	benchmark_arg arg[16];
	uintptr_t thread_handle[16];
	FILE* fd;

	char filebuf[64];
	sprintf(filebuf, "benchmark-random-small-%s.txt", benchmark_name());
	fd = fopen(filebuf, "w+b");

	for (size_t num_threads = 1; num_threads <= MAX_THREAD_COUNT; ++num_threads) {
		benchmark_start = 0;

		printf("Running %u threads alloc/free random size <4096: ", (unsigned int)num_threads);
		fflush(stdout);

		for (size_t ithread = 0; ithread < num_threads; ++ithread) {
			arg[ithread].size = 4096;
			arg[ithread].thread_arg.fn = allocate_random_size;
			arg[ithread].thread_arg.arg = &arg[ithread];
			thread_handle[ithread] = thread_run(&arg[ithread].thread_arg);
		}

		thread_sleep(1000);

		benchmark_start = 1;
		thread_fence();

		uint64_t mops = 0;
		uint64_t ticks = 0;
		for (size_t ithread = 0; ithread < num_threads; ++ithread) {
			thread_join(thread_handle[ithread]);
			ticks += arg[ithread].ticks;
			mops += arg[ithread].mops;
			if (!arg[ithread].accumulator)
				exit(-1);
		}

		if (!ticks)
			ticks = 1;

		double time_elapsed = timer_ticks_to_seconds(ticks);
		double average_mops = (double)mops / time_elapsed;
		char linebuf[128];
		int len = snprintf(linebuf, sizeof(linebuf), "%u,%u,%u\n", (unsigned int)num_threads,
		                   (unsigned int)average_mops, (unsigned int)get_process_memory_usage());
		fwrite(linebuf, (len > 0) ? (size_t)len : 0, 1, fd);
		fflush(fd);

		printf("%u memory ops/CPU second\n", (unsigned int)average_mops);
		fflush(stdout);
	}

	fclose(fd);

#if 0
	sprintf(filebuf, "benchmark-random-large-%s.txt", benchmark_name());
	fd = fopen(filebuf, "w+b");

	for (size_t num_threads = 1; num_threads <= MAX_THREAD_COUNT; ++num_threads) {
		benchmark_start = 0;

		printf("Running %u threads alloc/free random size <65536: ", (unsigned int)num_threads);
		fflush(stdout);

		for (size_t ithread = 0; ithread < num_threads; ++ithread) {
			arg[ithread].size = 0;
			arg[ithread].thread_arg.fn = allocate_random_size;
			arg[ithread].thread_arg.arg = &arg[ithread];
			thread_handle[ithread] = thread_run(&arg[ithread].thread_arg);
		}

		thread_sleep(1000);

		benchmark_start = 1;
		thread_fence();

		uint64_t mops = 0;
		uint64_t ticks = 0;
		for (size_t ithread = 0; ithread < num_threads; ++ithread) {
			thread_join(thread_handle[ithread]);
			ticks += arg[ithread].ticks;
			mops += arg[ithread].mops;
			if (!arg[ithread].accumulator)
				exit(-1);
		}

		if (!ticks)
			ticks = 1;

		double time_elapsed = timer_ticks_to_seconds(ticks);
		double average_mops = (double)mops / time_elapsed;
		char linebuf[128];
		int len = snprintf(linebuf, sizeof(linebuf), "%u,%u,%u\n", (unsigned int)num_threads,
		                   (unsigned int)average_mops, (unsigned int)get_process_memory_usage());
		fwrite(linebuf, (len > 0) ? (size_t)len : 0, 1, fd);
		fflush(fd);

		printf("%u memory ops/CPU second\n", (unsigned int)average_mops);
		fflush(stdout);
	}

	fclose(fd);
#endif

	size_t size_table[] = {
		16, 32, 48, 64, 96, 128,
		160, 192, 256,
		320, 384, 512,
		640, 768, 1024,
		1280, 1536, 2048,
		2560, 3072, 4096,
		5120, 6144, 8192,
		10240, 12288, 16384,
		20480, 24576, 32768,
		40960, 49152, 65536
	};
	const size_t num_sizes = sizeof(size_table) / sizeof(size_table[0]);

	for (size_t isize = 0; isize < num_sizes; ++isize) {
		size_t size = size_table[isize];

		sprintf(filebuf, "benchmark-fixed-%u-%s.txt", (unsigned int)size, benchmark_name());
		fd = fopen(filebuf, "w+b");

		for (size_t num_threads = 1; num_threads <= MAX_THREAD_COUNT; ++num_threads) {
			char linebuf[128];
			int len = snprintf(linebuf, sizeof(linebuf), "%u threads\n", (unsigned int)num_threads);
			fwrite(linebuf, (len > 0) ? (size_t)len : 0, 1, fd);
			fflush(fd);

			benchmark_start = 0;

			printf("Running %u threads allocating size %u: ", (unsigned int)num_threads, (unsigned int)size);
			fflush(stdout);

			for (size_t ithread = 0; ithread < num_threads; ++ithread) {
				arg[ithread].size = size;
				arg[ithread].thread_arg.fn = allocate_fixed_size;
				arg[ithread].thread_arg.arg = &arg[ithread];
				thread_handle[ithread] = thread_run(&arg[ithread].thread_arg);
			}

			thread_sleep(1000);

			benchmark_start = 1;
			thread_fence();

			uint64_t mops = 0;
			uint64_t ticks = 0;
			for (size_t ithread = 0; ithread < num_threads; ++ithread) {
				thread_join(thread_handle[ithread]);
				ticks += arg[ithread].ticks;
				mops += arg[ithread].mops;
				if (!arg[ithread].accumulator)
					exit(-1);
			}

			if (!ticks)
				ticks = 1;

			double time_elapsed = timer_ticks_to_seconds(ticks);
			double average_mops = (double)mops / time_elapsed;
			len = snprintf(linebuf, sizeof(linebuf), "%u,%u,%u\n",
			               (unsigned int)size, (unsigned int)average_mops, (unsigned int)get_process_memory_usage());
			fwrite(linebuf, (len > 0) ? (size_t)len : 0, 1, fd);
			fflush(fd);

			printf("%u memory ops\n", (unsigned int)average_mops);
			fflush(stdout);
		}

		fclose(fd);
	}

	if (benchmark_finalize() < 0)
		return -4;

	return 0;
}
