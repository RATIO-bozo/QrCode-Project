#include "makeqrcode.h"


char *init_matrix(int version, size_t *size_p){
    size_t nb_pixels = 21 + 4 * (version - 1);
    *size_p = nb_pixels;
    char *matrix = malloc(nb_pixels * nb_pixels * sizeof(char));

    if (matrix == NULL)
    {
        err(EXIT_FAILURE, "init_matrix: malloc()");
    }

    size_t size = nb_pixels * nb_pixels;

    for (size_t i = 0; i < size; i++)
    {
        matrix[i] = '2';
    }
    return matrix;
} 


void finder(char* mat, size_t size_p){
    __finder(mat, 0, 0, size_p);
    __finder(mat, size_p-7, 0, size_p);
    __finder(mat, 0, size_p-7, size_p);

    //separator

    for(size_t y=8; y<size_p-8; y++)
    {
		size_t x= 6;
		if(y%2==0)
			mat[(size_p*y+x)]='1';

		if(y%2==1)
			mat[(size_p*y+x)]='0';	
    }

    for(size_t x=8; x<size_p-8; x++)
    {
		size_t y= 6;
		if(x%2==0)
			mat[(size_p*y+x)]='1';

		if(x%2==1)
			mat[(size_p*y+x)]='0';

    }

    for(size_t y=0; y<8; y++)
    {
		size_t x=7;
		mat[(size_p*y+x)]='0';
		mat[(size_p*x+y)]='0';
		mat[size_p*x+y+size_p-8]='0';
		mat[size_p*(y+size_p-8)+x]='0';
    }

    //dark module
    
    mat[size_p*(size_p-8)+8]='1';

}

void __finder(char* mat, size_t i, size_t j, size_t size_p){
    for(size_t x=0; x<7; x++)
    {
        for(size_t y=0; y<7; y++)
        {	    		
            if( x>=2 && x <= 4 && y>=2 && y <= 4)
            {
                mat[(x+i)*size_p+y+j]='1';
            }
	    	   	    
			else
			{
				if(y == 0 || y == 6 || x == 0 || x == 6)
				mat[(x+i)*size_p+y+j]='1'; 

			
				else
				mat[(x+i)*size_p+y+j]='0';
	    	}			    
        }
    }
    //separator 
    
   if( j == 0 && i == 0)
   {
    
		for(size_t x = 0; x<8; x++)
		{
			size_t y = size_p-8;
			mat[(x+i)*size_p+y+j]='0';
		}
		for(size_t y = 0; y<8; y++)
		{
			size_t x = size_p-8;
			mat[(x+i)*size_p+y+j]='0';
		}
	}
   
}


void alignement(int version, char* mat, size_t size_p){

    int tab[][8]={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{6,18,0,0,0,0,0,0},{6,22,0,0,0,0,0,0},{6,26,0,0,0,0,0,0},{6,30,0,0,0,0,0,0},{6,34,0,0,0,0,0,0},{6,22,38,0,0,0,0,0},{6,24,42,0,0,0,0,0},{6,26,46,0,0,0,0,0},{6,28,50,0,0,0,0,0},{6,30,54,0,0,0,0,0},{6,32,58,0,0,0,0,0},{6,34,62,0,0,0,0,0},{6,26,46,66,0,0,0,0},{6,26,48,70,0,0,0,0},{6,26,50,74,0,0,0,0},{6,30,54,78,0,0,0,0},{6,30,56,82,0,0,0,0},{6,30,58,86,0,0,0,0},{6,34,62,90,0,0,0,0}};

    for(int i=0; tab[version][i]>0;i++)
    {
        for(int j= 0; tab[version][j]>0;j++)
        {
            int a = tab[version][i];
            int b = tab[version][j];

            int size = (int)size_p;

            //_align(a, b, mat, size_p);

            if(a == 6 && b == 6)
                continue;

            else if(a == size-7 && b == 6)
                continue;

            else if (a == 6 && b == size-7)
                continue;

            else
                _align(a, b, mat, size_p);


            /*_align(a, b, mat, size_p);

            if(mat[size_p*b+a-2]=='2' && mat[size_p*b+a-1]=='2' && mat[size_p*b+a+1]=='2' && mat[size_p*b+a+2]=='2' && mat[size_p*b+a]=='2')
            {
                _align(a, b, mat, size_p);	

            }

            if(mat[size_p*b+a-2]=='1' && mat[size_p*b+a-1]=='0' && mat[size_p*b+a+1]=='0' && mat[size_p*b+a+2]=='1' && mat[size_p*b+a]=='1')
            {
                _align(a, b, mat, size_p);	

            }

            if(mat[size_p*b+a-2]=='0' && mat[size_p*b+a-1]=='1' && mat[size_p*b+a+1]=='1' && mat[size_p*b+a+2]=='0' && mat[size_p*b+a]=='0')
            {
                _align(a, b, mat, size_p);	
        
            }
            */

            //int size;
            //size = (int) size_p;


            //MARCHE DE BASE 

            //if( mat[size_p*(a+2)+(b+2)] =='2' && mat[size_p*(a+2)+(b-2)] == '2' && mat[size_p*(a-2)+(b-2)] =='2' && mat[size_p*(a-2)+(b+2)] == '2')
            //    _align(a, b, mat, size_p);

            /*else
            {
                if((a!=6 && b!=6) && (a != size-6 && b!=6) && (a!=6 && b!= size-6))
                    _align(a, b, mat, size_p);	
                

            }	
            */
            
        }	
    }

}

void _align(int i, int j, char* mat, size_t size_p){

	j=j-2;
	i=i-2;

	for(size_t x=0; x<5; x++)
    {
        for(size_t y=0; y<5; y++)
        {	    		
            
			if(y == 0 || y == 4 || x == 0 || x == 4)
			{
				mat[(x+i)*size_p+y+j]='1'; 
				
			}

			else
			{
				if(x == 2 && y == 2)
					mat[(x+i)*size_p+y+j]='1';

				else
					mat[(x+i)*size_p+y+j]='0';

			}	 	 			    
        }
    }
}


void reserved(int version, char* matrix, size_t size){

    if(version < 7)
    {
		matrix[8 * size + 0] = '3';
        matrix[8 * size + 1] = '3';
        matrix[8 * size + 2] = '3';
        matrix[8 * size + 3] = '3';
        matrix[8 * size + 4] = '3';
        matrix[8 * size + 5] = '3';
        matrix[8 * size + 7] = '3';
        matrix[8 * size + 8] = '3';

        matrix[0 * size + 8] = '3';
        matrix[1 * size + 8] = '3';
        matrix[2 * size + 8] = '3';
        matrix[3 * size + 8] = '3';
        matrix[4 * size + 8] = '3';
        matrix[5 * size + 8] = '3';
        matrix[7 * size + 8] = '3';

        matrix[(size - 1) * size + 8] = '3';
        matrix[(size - 2) * size + 8] = '3';
        matrix[(size - 3) * size + 8] = '3';
        matrix[(size - 4) * size + 8] = '3';
        matrix[(size - 5) * size + 8] = '3';
        matrix[(size - 6) * size + 8] = '3';
        matrix[(size - 7) * size + 8] = '3';

        matrix[8 * size + size - 1] = '3';
        matrix[8 * size + size - 2] = '3';
        matrix[8 * size + size - 3] = '3';
        matrix[8 * size + size - 4] = '3';
        matrix[8 * size + size - 5] = '3';
        matrix[8 * size + size - 6] = '3';
        matrix[8 * size + size - 7] = '3';
        matrix[8 * size + size - 8] = '3';
    }


	if(version >= 7 )
	{

		for (size_t x = size - 11; x < size - 8; x++)
		{
			for (size_t y = 0; y < 6; y++)
			{
				matrix[x * size + y] = '3';
			}
		}
		for (size_t x = 0; x < 6; x++)
		{
			for (size_t y = size - 11; y < size - 8; y++)
			{
				matrix[x * size + y] = '3';
			}
		}
	}
}

void apply_more7(char *matrix, char s[], size_t size)
{
    size_t k = 17;
    for (size_t j = 0; j <= 5; j++)
    {
        for (size_t i = size - 11; i < size - 8; i++)
        {
            matrix[i * size + j] = s[k]+48;
            k--;
        }
    }

    k = 17;
    for (size_t i = 0; i <= 5; i++)
    {
        for (size_t j = size - 11; j < size - 8; j++)
        {
            matrix[i * size + j] = s[k]+48;
            k--;
        }
    }
}

void apply_less6(char *matrix, char s[], size_t size)
{
    size_t k = 0;

    matrix[8 * size + 0] = s[k]+48;
    k++;
    matrix[8 * size + 1] = s[k]+48;
    k++;
    matrix[8 * size + 2] = s[k]+48;
    k++;
    matrix[8 * size + 3] = s[k]+48;
    k++;
    matrix[8 * size + 4] = s[k]+48;
    k++;
    matrix[8 * size + 5] = s[k]+48;
    k++;
    matrix[8 * size + 7] = s[k]+48;
    k++;
    matrix[8 * size + 8] = s[k]+48;
    k++;

    matrix[7 * size + 8] = s[k]+48;
    k++;
    matrix[5 * size + 8] = s[k]+48;
    k++;
    matrix[4 * size + 8] = s[k]+48;
    k++;
    matrix[3 * size + 8] = s[k]+48;
    k++;
    matrix[2 * size + 8] = s[k]+48;
    k++;
    matrix[1 * size + 8] = s[k]+48;
    k++;
    matrix[0 * size + 8] = s[k]+48;
    k++;

    k = 0;
    matrix[(size - 1) * size + 8] = s[k]+48;
    k++;
    matrix[(size - 2) * size + 8] = s[k]+48;
    k++;
    matrix[(size - 3) * size + 8] = s[k]+48;
    k++;
    matrix[(size - 4) * size + 8] = s[k]+48;
    k++;
    matrix[(size - 5) * size + 8] = s[k]+48;
    k++;
    matrix[(size - 6) * size + 8] = s[k]+48;
    k++;
    matrix[(size - 7) * size + 8] = s[k]+48;
    k++;

    matrix[8 * size + size - 8] = s[k]+48;
    k++;
    matrix[8 * size + size - 7] = s[k]+48;
    k++;
    matrix[8 * size + size - 6] = s[k]+48;
    k++;
    matrix[8 * size + size - 5] = s[k]+48;
    k++;
    matrix[8 * size + size - 4] = s[k]+48;
    k++;
    matrix[8 * size + size - 3] = s[k]+48;
    k++;
    matrix[8 * size + size - 2] = s[k]+48;
    k++;
    matrix[8 * size + size - 1] = s[k]+48;
}


void reserved_area(char *m, int version, int mask, size_t size)
{
    if (version < 7)
    {
        if (mask == 0)
        {
            char s[] = { 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 };
            apply_less6(m, s, size);
        }
        else if (mask == 1)
        {
            char s[] = { 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1 };
            apply_less6(m, s, size);
        }
        else if (mask == 2)
        {
            char s[] = { 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0 };
            apply_less6(m, s, size);
        }
        else if (mask == 3)
        {
            char s[] = { 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1 };
            apply_less6(m, s, size);
        }
        else if (mask == 4)
        {
            char s[] = { 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1 };
            apply_less6(m, s, size);
        }
        else if (mask == 5)
        {
            char s[] = { 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0 };
            apply_less6(m, s, size);
        }
        else if (mask == 6)
        {
            char s[] = { 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1 };
            apply_less6(m, s, size);
        }
        else
        {
            char s[] = { 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };
            apply_less6(m, s, size);
        }
    }
    else
    {
        if (version == 7)
        {
            char s[] = { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0 };
            apply_more7(m, s, size);
        }
        else if (version == 8)
        {
            char s[] = { 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0 };
            apply_more7(m, s, size);
        }
        else if (version == 9)
        {
            char s[] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1 };
            apply_more7(m, s, size);
        }
        else if (version == 10)
        {
            char s[] = { 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1 };
            apply_more7(m, s, size);
        }
    }
}



void upward_placement(char *matrix, size_t size, int *code, size_t *index,size_t *x, size_t *y){
    size_t n = size * 2;
    for (size_t i = 0; i < n - 1; i++)
    {
        if (matrix[(*x) * size + (*y)] == '2')
        {
            matrix[(*x) * size + (*y)] = code[*index]+48;
            *index += 1;
        }
        if (i % 2 == 0)
        {
            *y -= 1;
        }
        else
        {
            *x -= 1;
            *y += 1;
        }
    }
    if (matrix[(*x) * size + (*y)] == '2')
    {
        matrix[(*x) * size + (*y)] = code[*index]+48;
        *index += 1;
    }
}

void downward_placement(char *matrix, size_t size, int *code, size_t *index,size_t *x, size_t *y, size_t enc){
    size_t n = size * 2;
    if (*y == 6)
        *y -= 1;
    for (size_t i = 0; i < n - 1; i++)
    {
        if (matrix[(*x) * size + (*y)] == '2')
        {
            if (enc == *index)
                matrix[(*x) * size + (*y)] = '0';
            else
            {
                matrix[(*x) * size + (*y)] = code[*index]+48;
                *index += 1;
            }
        }
        if (i % 2 == 0)
        {
            *y -= 1;
        }
        else
        {
            *x += 1;
            *y += 1;
        }
    }

    if (matrix[(*x) * size + (*y)] == '2')
    {
        if (enc == *index)
            matrix[(*x) * size + (*y)] = '0';
        else
            matrix[(*x) * size + (*y)] = code[*index]+48;
        *index += 1;
    }
}

void fillmatrix(char *matrix, size_t size, int *code, size_t enc){
    size_t n = (size - 1) / 2;
    size_t index = 0;
    size_t x = size - 1;
    size_t y = size;
    for (size_t i = 0; i < n; i++)
    {
        y -= 1;
        if (i % 2 == 0)
            upward_placement(matrix, size, code, &index, &x, &y);
        else
        {
            downward_placement(matrix, size, code, &index, &x, &y, enc);
        }
    }
}


