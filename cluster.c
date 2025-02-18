/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif
 
/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};


/*Added funcion*/


/*int IsCountValid(char *arr_of_count){
for (int i = 0; i<strlen(arr_of_count); i++)
{
    if (arr_of_count[i] = 'c'||
|| arr_of_count[i] == '='
|| arr_of_count[i] >= '0'
&& arr_of_count[i] <= '9')
{
    return *arr_of_count;
}else
{
    printf("Invalid count!");
}
}

}*/

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
    c->size = 0;
    if (cap == 0)
    {
        c->capacity = cap;
        c->obj = NULL;
        return;
    }
    c->obj = malloc(sizeof(struct obj_t) * cap);
    if (c->obj == NULL)
    {
        fprintf(stderr, "Allocation error");
        return;
    }
    c->capacity = cap;
}
/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    init_cluster(c , 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    assert(c);
    assert(c->size >= 0);
    if (c->size == c->capacity)
    {
         resize_cluster(c, c->capacity + CLUSTER_CHUNK);
	}
    c->obj[c->size++] = obj;
}


/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
    
    for (int i = 0; i < c2->size; i++)
{
    append_cluster(c1, c2->obj[i]);
}
sort_cluster(c1);
    
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
    if (idx >= 0)
    {
        clear_cluster(&carr[idx]);
    }
    int new_narr = narr - 1;
    for (int i = idx; i < new_narr; i++)
    {
        carr[i] = carr[i + 1];
    }
    return new_narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
float distance_obj = sqrtf((o1->x - o2->x)*(o1->x - o2->x) + (o1->y - o2->y)*(o1->y - o2->y));
    return distance_obj; 
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);
float distance_cluster;
float min_distance = 1414.21356;
for (int i = 0; i < c1->size; i++)
{
    for (int j = 0; j < c2->size; j++)
    {
        distance_cluster = obj_distance(&c1->obj[i] ,&c2->obj[j]);
       if (min_distance >= distance_cluster)
       {
        min_distance = distance_cluster;
       }
       
    }
}
return min_distance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
if (narr == 1)
{
    *c1 = *c2 = 0;
    return;
}
float min_distance = 1414.21356;
float clusters_distance;
    for (int i = 0; i < narr; i++)
    {
        for (int j = i + 1; j < narr; j++)
        {
            clusters_distance = cluster_distance(&carr[i], &carr[j]);
            if (min_distance > clusters_distance)
            {
                min_distance = clusters_distance;
                *c1 = i;
                *c2 = j;
            }
            
        }
        
    }
    
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++) {
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}
/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
FILE *file = fopen(filename, "r");
if ( ! file) {
		fprintf(stderr,"File %s isn't opened",filename);
		return -1;
	}
int id;
float x,y;
struct obj_t obj;
struct cluster_t *cluster;
if (file == NULL)
{
    fprintf(stderr,"There aren't any objects in file");
    return -1;
}
//Read count
int count = 0,number_of_line = 0;
char arr_of_count[15];
fgets(arr_of_count, 15 , file);
char *token;
token = strtok(arr_of_count,"count=");
while (token != NULL )
{
   if((count = atoi(token)) == 0)
   {
    fprintf(stderr,"wrong count");
    return -1;
   }
   token = strtok(NULL,"count=");
} 
//Read obj
char str[100], endchar;
*arr = malloc(count * sizeof(struct cluster_t));

for (int i = 0; i < count; i++)
{
    init_cluster(&(*arr)[i], 1);
}

while(fgets(str , 100 , file))
{
    if (number_of_line + 1 > count)
    {
        number_of_line++;
    break;
    }
  for (unsigned j = 0; j < strlen(str);j++)
  {
    if (!(str[j] == ' ' || str[j] == '\n'
  || (str[j]>= '0' && str[j] <= '9')))
  {
    fprintf(stderr,"Object:%s is not correct ",str);
    return -1;
  }
  }

if(sscanf(str,"%d %f %f %[^\n]",&id,&x,&y, &endchar) != 3)
{
    fprintf(stderr,"Object:%s is not correct ",str);
    return -1;
}
if ((x >= 0 && x <= 1000) && (y >= 0 && y <= 1000))
{
    obj.id = id;
    obj.x = x;
    obj.y = y;
    //Unik id
    
    
}else
{
    fprintf(stderr,"Incorrect x or y in object:%s",str);
    return -1;
}
cluster = &(*arr)[number_of_line];
append_cluster(cluster, obj);
number_of_line++;

}
if (count != number_of_line)
{
    fprintf(stderr,"Invalid count = %d",count);
    return -1;
}

fclose(file);
return count;
}



int new_clusters_size(struct cluster_t *clusters , int size , int new_size){
assert(size >= 0);
assert(new_size > 0);
 if(new_size > size)
{
    fprintf(stderr,"Number of clusters are more then number of objects");
    return -1;
}
//int c1_size;
int index_c1 , index_c2;
while (size > new_size)
{
    find_neighbours(clusters,size,&index_c1,&index_c2);
    //c1_size = clusters[index_c1].size;
    merge_clusters(&clusters[index_c1], &clusters[index_c2]);
    size = remove_cluster(clusters,size , index_c2);
}
return size;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]){
    struct cluster_t *clusters;
    int size, new_size = 1;

if (argc > 1 && argc < 4)
{
    size = load_clusters(argv[1],&clusters);
    if(size == -1)
    {
    fprintf(stderr,"Number of clusters is wrong");
    return -1;
    }
    
    
    if (argc == 2)
    {
    size = new_clusters_size(clusters,size,new_size);
    print_clusters(clusters, size);
    
    for (int i = 0; i < new_size; i++)
    {
        clear_cluster(&clusters[i]);
    }
    free(clusters);
    }
    
    
    
    else if (argc == 3)
{

char *N = argv[2];
for ( int i = 0; (i < N[i]) != '\0'; i++)
{                         
    if ( N[i] < '0' || N[i] > '9')
    {
         fprintf(stderr,"Incorrect N!");
        return -1;
    }
    
}
new_size = atoi(argv[2]);

if (new_size > 0 || size >new_size)
{
    size = new_clusters_size(clusters,size,new_size);
    print_clusters(clusters, new_size);
    
    for (int i = 0; i < size; i++)
    {
        clear_cluster(&clusters[i]);
    }
    free(clusters);
    
} else
    {
    fprintf(stderr,"Incorrect N!");
    return -1;
    }
}

}
else
{
    fprintf(stderr,"Wrong number of arguments!");
    return 1;
}
return 0;
}
