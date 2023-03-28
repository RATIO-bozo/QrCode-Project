#include "qrmask.h"

struct mask *init_mask(char *qr, size_t size)
{
    struct mask *mask = malloc(sizeof(struct mask));
    if (mask == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask0 = malloc(size * size);
    if (mask->mask0 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask1 = malloc(size * size);
    if (mask->mask1 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask2 = malloc(size * size);
    if (mask->mask2 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask3 = malloc(size * size);
    if (mask->mask3 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask4 = malloc(size * size);
    if (mask->mask4 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask5 = malloc(size * size);
    if (mask->mask5 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask6 = malloc(size * size);
    if (mask->mask6 == NULL)
        err(EXIT_FAILURE, "mask");

    mask->mask7 = malloc(size * size);
    if (mask->mask7 == NULL)
        err(EXIT_FAILURE, "mask");

    for (size_t i = 0; i < size * size; i++)
    {
        if (qr[i] == '3')
        {
            mask->mask0[i] = '0';
            mask->mask1[i] = '0';
            mask->mask2[i] = '0';
            mask->mask4[i] = '0';
            mask->mask3[i] = '0';
            mask->mask5[i] = '0';
            mask->mask6[i] = '0';
            mask->mask7[i] = '0';
        }
        else
        {
            mask->mask0[i] = qr[i];
            mask->mask1[i] = qr[i];
            mask->mask2[i] = qr[i];
            mask->mask3[i] = qr[i];
            mask->mask4[i] = qr[i];
            mask->mask5[i] = qr[i];
            mask->mask6[i] = qr[i];
            mask->mask7[i] = qr[i];
        }
    }

    return mask;
}

void apply_mask(struct mask *mask, char *m, size_t size,int version)
{
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if ((i + j) % 2 == 0)
            {
                mask->mask0[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if (i % 2 == 0)
            {
                mask->mask1[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if (j % 3 == 0)
            {
                mask->mask2[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if ((i + j) % 3 == 0)
            {
                mask->mask3[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if (((i / 2) + (j / 3)) % 2 == 0)
            {
                mask->mask4[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if ((i * j) % 2 + (i * j) % 3 == 0)
            {
                mask->mask5[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if (((i * j) % 2 + (i * j) % 3) % 2 == 0)
            {
                mask->mask6[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
            if (((i * j) % 3 + (i + j) % 2) % 2 == 0)
            {
                mask->mask7[i * size + j] = ((m[i * size + j] + 1) % 2)+48;
            }
        }
    }
    finder(mask->mask0,size);
    alignement(version,mask->mask0 , size);
    reserved(version,mask->mask0,size);

    finder(mask->mask1,size);
    alignement(version,mask->mask1 , size);
    reserved(version,mask->mask1,size);

    finder(mask->mask2,size);
    alignement(version,mask->mask2 , size);
    reserved(version,mask->mask2,size);

    finder(mask->mask3,size);
    alignement(version,mask->mask3 , size);
    reserved(version,mask->mask3,size);

    finder(mask->mask4,size);
    alignement(version,mask->mask4 , size);
    reserved(version,mask->mask4,size);

    finder(mask->mask5,size);
    alignement(version,mask->mask5 , size);
    reserved(version,mask->mask5,size);

    finder(mask->mask6,size);
    alignement(version,mask->mask6 , size);
    reserved(version,mask->mask6,size);

    finder(mask->mask7,size);
    alignement(version,mask->mask7 , size);
    reserved(version,mask->mask7,size);
}

void free_mask(struct mask *mask)
{
    free(mask->mask0);
    free(mask->mask1);
    free(mask->mask2);
    free(mask->mask3);
    free(mask->mask4);
    free(mask->mask5);
    free(mask->mask6);
    free(mask->mask7);
    free(mask);
}

int condition_1(char *matrix, size_t size)
{
    int p = 0;
    int cb = 0;
    int cd = 0;
    for (size_t i = 0; i < size; i++)
    {
        int b = matrix[i * size];
        int d = matrix[i];
        cb = 1;
        cd = 1;
        for (size_t j = 1; j < size; j++)
        {
            int t = matrix[i * size + j];
            int f = matrix[j * size + i];
            if (b == t)
            {
                cb++;
                if (cb == 5)
                    p += 3;
                else if (cb > 5)
                    p++;
            }
            else
                cb = 0;
            b = t;
            if (d == f)
            {
                cd++;
                if (cd == 5)
                    p += 3;
                else if (cd > 5)
                    p++;
            }
            else
                cd = 0;
            d = f;
        }
    }
    return p;
}

int condition_2(char *matrix, size_t size)
{
    int p = 0;
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - 1; j++)
        {
            if (matrix[i * size + j] && matrix[(i + 1) * size + j]
                && matrix[i * size + j + 1] && matrix[(i + 1) * size + j + 1])
                p += 3;
            else if (!matrix[i * size + j] && !matrix[(i + 1) * size + j]
                     && !matrix[i * size + j + 1]
                     && !matrix[(i + 1) * size + j + 1])
                p += 3;
        }
    }
    return p;
}

int condition_3(char *matrix, size_t size)
{
    int p = 0;
    for (size_t i = 0; i < size - 10; i++)
    {
        for (size_t j = 0; j < size - 10; j++)
        {
            if (matrix[i * size + j] % 2 == 1 && matrix[i * size + j + 1] == 0
                && matrix[i * size + j + 2] % 2 == 1
                && matrix[i * size + j + 3] % 2 == 1
                && matrix[i * size + j + 4] % 2 == 1
                && matrix[i * size + j + 5] == 0
                && matrix[i * size + j + 6] % 2 == 1
                && matrix[i * size + j + 7] == 0
                && matrix[i * size + j + 8] == 0
                && matrix[i * size + j + 9] == 0
                && matrix[i * size + j + 10] == 0)
                p += 40;
            else if (matrix[i * size + j] == 0 && matrix[i * size + j + 1] == 0
                     && matrix[i * size + j + 2] == 0
                     && matrix[i * size + j + 3] == 0
                     && matrix[i * size + j + 4] % 2 == 1
                     && matrix[i * size + j + 5] == 0
                     && matrix[i * size + j + 6] % 2 == 1
                     && matrix[i * size + j + 7] % 2 == 1
                     && matrix[i * size + j + 8] % 2 == 1
                     && matrix[i * size + j + 9] == 0
                     && matrix[i * size + j + 10] % 2 == 1)
                p += 40;
        }
    }
    return p;
}

int abs(int a)
{
    if (a < 0)
        a = -a;
    return a;
}

int condition_4(char *matrix, size_t size)
{
    size_t t = size * size;
    size_t b = 0;
    for (size_t i = 0; i < t; i++)
    {
        if (matrix[i])
            b++;
    }
    size_t pourcentage = b * 100 / t;
    int m_prev = pourcentage - (pourcentage % 5);
    int m_next = m_prev + 5;
    m_prev = abs(m_prev - 50) / 5;
    m_next = abs(m_next - 50) / 5;
    if (m_prev <= m_next)
        return m_prev * 10;
    return m_next * 10;
}

int mask_final(char **m, size_t nb_pixel, struct mask *mask,int version)
{
    int vmask = 0;
    apply_mask(mask, *m, nb_pixel,version);
    int p0 = condition_1(mask->mask0, nb_pixel)
        + condition_2(mask->mask0, nb_pixel)
        + condition_3(mask->mask0, nb_pixel)
        + condition_4(mask->mask0, nb_pixel);
    int p1 = condition_1(mask->mask1, nb_pixel)
        + condition_2(mask->mask1, nb_pixel)
        + condition_3(mask->mask1, nb_pixel)
        + condition_4(mask->mask1, nb_pixel);
    int p2 = condition_1(mask->mask2, nb_pixel)
        + condition_2(mask->mask2, nb_pixel)
        + condition_3(mask->mask2, nb_pixel)
        + condition_4(mask->mask2, nb_pixel);
    int p3 = condition_1(mask->mask3, nb_pixel)
        + condition_2(mask->mask3, nb_pixel)
        + condition_3(mask->mask3, nb_pixel)
        + condition_4(mask->mask3, nb_pixel);
    int p4 = condition_1(mask->mask4, nb_pixel)
        + condition_2(mask->mask4, nb_pixel)
        + condition_3(mask->mask4, nb_pixel)
        + condition_4(mask->mask4, nb_pixel);
    int p5 = condition_1(mask->mask5, nb_pixel)
        + condition_2(mask->mask5, nb_pixel)
        + condition_3(mask->mask5, nb_pixel)
        + condition_4(mask->mask5, nb_pixel);
    int p6 = condition_1(mask->mask6, nb_pixel)
        + condition_2(mask->mask6, nb_pixel)
        + condition_3(mask->mask6, nb_pixel)
        + condition_4(mask->mask6, nb_pixel);
    int p7 = condition_1(mask->mask7, nb_pixel)
        + condition_2(mask->mask7, nb_pixel)
        + condition_3(mask->mask7, nb_pixel)
        + condition_4(mask->mask7, nb_pixel);
    int t[] = { p0, p1, p2, p3, p4, p5, p6, p7 };
    int pt = p0;
    int v = 0;
    for (int i = 1; i < 8; i++)
    {
        //printf("%d ", t[i]);

        if (t[i] < pt)
        {
            v = i;
            pt = t[i];
        }


    }
    
    //printf("\n %d \n", v);

    if (v == 0)
    {
        *m = mask->mask0;
        vmask = 0;
    }
    if (v == 1)
    {
        *m = mask->mask1;
        vmask = 1;
    }
    if (v == 2)
    {
        *m = mask->mask2;
        vmask = 2;
    }
    if (v == 3)
    {
        *m = mask->mask3;
        vmask = 3;
    }
    if (v == 4)
    {
        *m = mask->mask4;
        vmask = 4;
    }
    if (v == 5)
    {
        *m = mask->mask5;
        vmask = 5;
    }
    if (v == 6)
    {
        *m = mask->mask6;
        vmask = 6;
    }
    if (v == 7)
    {
        *m = mask->mask7;
        vmask = 7;
    }
    return vmask;
}
