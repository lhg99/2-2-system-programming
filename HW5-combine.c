/* combine.c, checking performance difference, Hyungi lee, lee991229@naver.com, 2021/12/15 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define CONFIG_FOR_MUL
#ifdef CONFIG_FOR_MUL
#define OPER *
#define IDENT 1
#endif

typedef long data_t;
typedef struct {
	int len;
	data_t *data;
}vec_rec, *vec_ptr;

vec_ptr new_vec(int len) {
	vec_ptr result = (vec_ptr)malloc(sizeof(vec_rec));
	if(!result)
		return NULL;
	result->len = len;
	if(len>0) {
		data_t *data = (data_t*)calloc(len, sizeof(data_t));
		if(!data) {
			free((void *) result);
			return NULL;
		}
		result->data = data;
		}
	else
		result->data = NULL;
	return result;
}

int get_vec_element(vec_ptr v, int index, data_t *dest) {
	if(index<0||index>=v->len)
		return 0;
	*dest = v->data[index];
	return 1;
}

int vec_length(vec_ptr v) {
	return v->len;
}

data_t *get_vec_start(vec_ptr v) {
	return v->data;
}

void combine1(vec_ptr v, data_t *dest) {
	int i;
	
	*dest = IDENT;
	for(i=0;i<vec_length(v);i++) {
		data_t val;
		get_vec_element(v, i, &val);
		*dest = *dest OPER val;
	}
}

void combine2(vec_ptr v, data_t *dest) {
	int i;
	int length = vec_length(v);
	
	*dest = IDENT;
	for(i=0;i<length;i++) {
		data_t val;
		get_vec_element(v, i, &val);
		*dest = *dest OPER val;
	}
}

void combine3(vec_ptr v, data_t *dest) {
        int i;
        int length = vec_length(v);
	data_t *data = get_vec_start(v);

        *dest = IDENT;
        for(i=0;i<length;i++) {
                *dest = *dest OPER data[i];
        }
}

void combine4(vec_ptr v, data_t *dest) {
        int i;
        int length = vec_length(v);
        data_t *data = get_vec_start(v);
	data_t x = IDENT;

        for(i=0;i<length;i++) {
		x = x OPER data[i];
        }
	*dest = x;
}

int main(int argc, char *argv[])
{
	int i, choice = 1, loop = 0;
	struct timeval stime, etime, gap;
	vec_ptr instance;
	data_t com_result = 0;
	void (*func_ptr)(vec_ptr, data_t *);

	if (argc == 3) {
		choice = atoi(argv[1]);
		loop = atoi(argv[2]);
		if(choice == 1)
			func_ptr = combine1;
		else if(choice == 2)
			func_ptr = combine2;
		else if(choice == 3)
			func_ptr = combine3;
		else if(choice == 4)
			func_ptr = combine4;
		else
			printf("you can choose 1~4");
		
		instance = new_vec(loop);

		for(i=0;i<loop;i++) {
			gettimeofday(&stime, NULL);
			(func_ptr)(instance, &com_result);
			gettimeofday(&etime, NULL);
		}
		gap.tv_sec = etime.tv_sec - stime.tv_sec;
		gap.tv_usec = etime.tv_usec - stime.tv_usec;

		if(gap.tv_usec < 0) {
			gap.tv_sec = gap.tv_sec - 1;
			gap.tv_usec = gap.tv_usec + 1000000;
		}
		printf("Elapsed time %ldsec:%ldusec\n", gap.tv_sec, gap.tv_usec);
		printf("Combine_result = %d\n", com_result);
	}
	else
		printf("Usage: command number loop_size\n");
}
