
#define NOISE_type 1
#define N 1024
#define HALF_h 256 
#define h 512
#define DIFF_secret 103
#define DIFF_noise 43
#define DIFF_noise_typeII 57
#define LENGTH_noise_typeII 97

#define TESTNUM  384000000
#define ThresNum 17 //threshold of decryption error
#define Thres 125
#define ETA 800 //lenth of the actural codes.
#define ETA_half 400
#define u64 unsigned long long int
#define r() ((u64)(rc4()) * (u64)(4294967296) + (u64)(rc4()) * (1<<24) +(u64)(rc4()) * 65536 + (u64)(rc4()) * 256 + (u64)(rc4()))
#define SIZE_NOISE sizeof(char[N])
#define SIZE_ETA sizeof(char[ETA])

typedef struct 
{
    char e1[N];
    char r[N];
    char e2[N];
    char Round_noise[ETA];
}record;




// generate a uniform distribution over (-7, 8]
char uniform15();
char LAC_noise();


// generate sparse secret of a 1 and b -1 of length l
void generate_sparse_secret(char *s, int a, int b, int l);
void generate_sparse_secrect_bias(char *s, int diff);

// the last entries are all 1 in the even position and all -1 in the odd position
void generate_bias_nosie(char *s, int l, int k);
void generate_bias_nosie_typeII(char *s, int l, int l1_half, int l0_half);
void generate_vec_LAC_noise(char *s, int l);
void generate_vec_rounding_noise(char *s, int l);

int SumEven(char* s, int l);

int SumOdd(char* s, int l);

// decryption errors if ThresNum positions are no less than than Thres
int decode(char *s, char *e, record *b, int *err, int *errPosi, int *errPosi_consitioned);
void printVector(int *s, int l);
