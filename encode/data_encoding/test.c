#include "encode_string.h"
#include "polynomial.h"
#include "makeqrcode.h"
#include "final_struct.h"

void print_struct(struct encdata *qr){
    printf("version = %d\n", qr->version);
    printf("mode indicator = %d\n", qr->mi);
    printf("size of input string = %lu\n", qr->size);
    printf("corection_level = %c\n", qr->correction_level);
    printf("length of data = %ld\n", qr->len);
    printf("data = %s\n", qr->data);
    //more later;
}

void print_matrix(char *matrix, size_t *size_p)
{
    printf("=================================================\n");
    size_t size = *size_p;
    for (size_t i = 0; i < *size_p; i++)
    {
        for (size_t j = 0; j < *size_p; j++)
        {
            if (matrix[i * size + j] == '0')
                printf(" 0");
            else if (matrix[i * size + j] == '1')
            {
                printf(" 1");
            }
            else
            {
                printf(" 2" );
            }
        }
        printf("\n");
    }
    printf("=================================================\n");
}


int main(int argc,char *argv[]){
    if(argc != 2){
	errx(EXIT_FAILURE,"Too few/much argument");
    }
    char* s=argv[1] ;
    char c;
    struct encdata *qr = data_encoding(s, "M");
    print_struct(qr);
    scanf("%c",&c);

    // Encode Message polynomial 
    struct poly ** message = build_message_polynomial(qr);
    printf("numbers of message polynomial needed : %ld \n",qr->block1+qr->block2); 
    for(size_t i = 0; i<qr->block1+qr->block2;i++){
	print_polynomial(message[i]);
    }
    scanf("%c",&c);

    //Encode Generator polynomial 
    struct poly* generator = build_generator_polynomial(qr->ec); 
    print_polynomial(generator);
    scanf("%c",&c);

    int* final = final_struct(qr);
    size_t t = qr->block1+qr->block2;
    size_t f = final_byte(qr->version);
    size_t all = (qr->block1*qr->group1+qr->block2*qr->group2+t*qr->ec)*8+f;
    printf("==============================Final data==============================\n\n");
    for(size_t i=0;i<all;i++){
	printf("%d",final[i]);
    }
   printf("\n\n=====================================================================\n\n");
    scanf("%c",&c);

    size_t test1;
    char* matrixtest = init_matrix(qr->version, &test1);
    print_matrix(matrixtest, &test1);
    scanf("%c",&c);
    finder(matrixtest, test1);
    print_matrix(matrixtest, &test1);
    scanf("%c",&c);
    alignement(qr->version, matrixtest,test1);
    print_matrix(matrixtest, &test1);
    scanf("%c",&c);
    test1 = 0;
    matrixtest = init_matrix(7,&test1);
    finder(matrixtest, test1);
    alignement(7, matrixtest,test1);
    print_matrix(matrixtest, &test1);



    return 1;
}
