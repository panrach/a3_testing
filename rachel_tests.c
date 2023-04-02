#include "ingredient_graph.c"

void make_list()
{
	strcpy(ingredients[0], "soymilk");
	strcpy(ingredients[1], "soymilk sweetened");
	strcpy(ingredients[2], "soymilk unsweetened");
	strcpy(ingredients[3], "chocolate soymilk");
	strcpy(ingredients[4], "strawberry soymilk");
	strcpy(ingredients[5], "coconut soymilk");
	strcpy(ingredients[6], "soymilk smoothie");
	strcpy(ingredients[7], "soymilk dessert");
	strcpy(ingredients[8], "vanilla soymilk");
	strcpy(ingredients[9], "malt soymilk");
}

void clear_list()
{
	for (int i = 0; i < 10; i++)
	{
		ingredients[i][0] = '\0';
	}
}

void test_print_ingredients()
{
	// 1.1: 10 ingredient list
	printf("1.1: do it yoself \n");
	intNode *head=NULL;
	make_list();
	
	for (int i = 10; i >= 0; i--)
	{
		head = insertInt(head, i);
	}
	// this may crash because your program could be accessing an index out of bound
	// it's safe to put a check in your program for that
	
 	printf("YOUR PRINT: \n");
	print_ingredients(head);
	printf("\nEXPECTED PRINT: \n");
	printf("soymilk\nsoymilk sweetened\nsoymilk unsweetened\nchocolate soymilk\nstrawberry soymilk\ncoconut soymilk\nsoymilk smoothie\nsoymilk dessert\nvanilla soymilk\nmalt soymilk\n");
	
	head = deleteList(head);
}

void test_ingredient_index()
{
	// 2.1: empty list
	// 2.2: 10 elements, not found
	// 2.3: 10 elements, found
	// 2.4: 10 elements, found in the middle
	
	// 2.1
	clear_list();
	printf("2.1: %s", ingredient_index("soymilk") == -1 ? "PASSED\n" : "FAILED\n");
	
	// 2.2
	make_list();
	printf("2.2: %s", ingredient_index("normal milk") == -1 ? "PASSED\n" : "FAILED\n");
	
	// 2.3
	printf("2.3: %s", ingredient_index("malt soymilk") == 9 ? "PASSED\n" : "FAILED\n");
	
	// 2.4
	printf("2.4: %s", ingredient_index("strawberry soymilk") == 4 ? "PASSED\n" : "FAILED\n");
}


void ghetto_load_ingredients(void)
{
  // Loads the adjacency matrix and ingredients list from file,
  // N must be either 10 (small graph), or 400 (full-size).
  
  FILE *f;
  int n=MAT_SIZE;
  
  if (n==10) f=fopen("AdjMat_small.dat","rb");
  else f=fopen("AdjMat_full.dat","rb");
  if (f==NULL)
  {
    printf("Can not open file with adjacency matrix. Please make sure it's in this directory\n");
    return;
  }
  fread(&AdjMat[0][0],n*n*sizeof(double),1,f);
  fclose(f);
  
  if (n==10) f=fopen("Ingredient_names_small.txt","r");
  else f=fopen("Ingredient_names_full.txt","r");
  if (f==NULL)
  {
    printf("Can not open file with ingredient names. Please make sure it's in this directory\n");
    return;
  }
 // printf("Reading ingredient names. Available ingredient are:\n");
  for (int i=0; i<n; i++)
  {
    fgets(&ingredients[i][0],MAX_STR_LEN,f);
    for (int j=0; j<strlen(ingredients[i]); j++)
      if (ingredients[i][j]==10||ingredients[i][j]==13) ingredients[i][j]=0;
    //printf("%s\n",ingredients[i]);
  }
  fclose(f);
}

void print_adj()
{
	// this functions prints the adj matrix
	ghetto_load_ingredients();
	for (int row = 0; row < MAT_SIZE; row++)
	{
		for (int col = 0; col < MAT_SIZE; col++)
		{
			printf("%2.0f ", AdjMat[row][col]);
		}
		printf("\n");
	}

}

void test_related_ingredients()
{
	// self test (check this whole matrix to be safe im too lazy to write it all out)
	// work out what the matrix should look like and input each ingredient
	
	printf("3.1: do it yoself \n");
	ghetto_load_ingredients();
	//print_adj();
	
	printf("\nEXPECTED PRINT:\nartichoke hearts\nsunflower seeds");
	printf("\n\nYOUR PRINT:\n");
	related_ingredients("quinoa");
}

void sort(char input_str[1024])
{
	int i, j;
	char temp;
	int len = strlen(input_str);
	
	for (i = 0; i < len - 1; i++)
	{
		for (j = i + 1; j < len; j++)
		{
			if (input_str[i] > input_str[j]) 
			{
				temp = input_str[i];
				input_str[i] = input_str[j];
				input_str[j] = temp;
			}
		}
	}
}

int compare_linked_list(intNode *actual, const char *expected)
{
	char buffer[1024];
	intNode *current = NULL;
	
	current = actual;
	buffer[0] = '\0';
	
	int rc;
	
	while (current != NULL)
	{
		strcat(buffer, ingredients[current->x]);
		strcat(buffer, ",");
		
		current = current->next;
	}
	
	sort(buffer);
	rc = strcmp(buffer, expected);
	
	if (rc != 0)
	{
		//printf("expected: %s actual: %s\n", expected, buffer);

	}
	
	return rc;
}

void test_k_dist()
{
	// 4.1: Empty matrix
	// 4.2: Distance 1 from soymilk
	// 4.3: Distance 2 from soymilk
	// 4.4: Distance 5 from artichoke hearts
	// 4.5: Distance 3 from turkey
	// 4.6: k greater than the size of matrix
	
	char expected[1024];
	memset(AdjMat, 0, sizeof(AdjMat));
	
	// 4.1
	intNode *head=NULL;
    head = related_k_dist(head,"soymilk",11,0);
	printf("4.1: %s", head == NULL ? " PASSED\n" : " FAILED\n");
	
	// 4.2 
	ghetto_load_ingredients();
	head = related_k_dist(head,"soymilk",1,0);
	strcpy(expected, "sunflower seeds,");
	sort(expected);
	printf("4.2: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 4.3
	head = deleteList(head);
	head =  related_k_dist(head,"soymilk",2,0);
	strcpy(expected, "quinoa,soymilk,sunflower seeds,");
	sort(expected);
	printf("4.3: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 4.4
	head = deleteList(head);
	head = related_k_dist(head,"artichoke hearts",5,0);
	strcpy(expected, "soymilk,sunflower seeds,quinoa,turkey,artichoke hearts,gruyere cheese,fresh breadcrumbs,tomato juice,prepared horseradish,medium shrimp,");
	sort(expected);
	printf("4.4: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 4.5
	head = deleteList(head);
	head = related_k_dist(head,"turkey",3,0);
	strcpy(expected, "artichoke hearts,medium shrimp,prepared horseradish,gruyere cheese,fresh breadcrumbs,quinoa,sunflower seeds,turkey,");
	sort(expected);
	printf("4.5: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 4.6
	head = deleteList(head);
	head=related_k_dist(head,"soymilk",13,0);
	strcpy(expected, "turkey,gruyere cheese,fresh breadcrumbs,tomato juice,prepared horseradish,medium shrimp,artichoke hearts,quinoa,soymilk,sunflower seeds,");
	sort(expected);
	printf("4.6: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
}

void test_related_with_restrictions()
{
	// 5.1: Empty matrix
	// 5.2: Distance 1 from soymilk. Avoid ingredient dne
	// 5.3: Distance 1 from soymilk. Avoid quinoa distance 1
	// 5.4: Distance 4 from soymilk. Avoid quinoa distance 1
	// 5.5: Avoid distance bigger than source distance. Distance 3 from soymilk. Avoid quinoa distance 4
	// 5.6: Avoid distance bigger than source distance. Distance 1 from cheese. Avoid soymilk distance 4
	// 5.7: Avoid distance bigger than the whole matrix. Distance 1 from cheese. Avoid soymilk distance 11
	// 5.8: Source distance bigger than the whole matrix. Distance 11 from soymilk. Avoid quinoa distance 1
	// 5.9: Distance 2 from turkey. Avoid soymilk distance 1
	// 5.10: both k values bigger than the matrix
	// 5.11: avoid value 0
	
	char expected[1024];
	memset(AdjMat, 0, sizeof(AdjMat));
	
	// 5.1
	intNode *head=NULL;
	head = deleteList(head);
	head = related_with_restrictions("soymilk", "sesame", 1, 1);
	printf("5.1: %s", head == NULL ? " PASSED\n" : " FAILED\n");
	
	// 5.2
	ghetto_load_ingredients();
	head = deleteList(head);
	head = related_with_restrictions("soymilk", "sesame", 1, 1);
	strcpy(expected, "sunflower seeds,");
	sort(expected);
	printf("5.2: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.3
	head = deleteList(head);
	head = related_with_restrictions("soymilk", "quinoa", 1, 1);
	strcpy(expected, "");
	sort(expected);
	printf("5.3: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.4
	head = deleteList(head);
	head = related_with_restrictions("soymilk", "quinoa", 4, 1);
	strcpy(expected, "soymilk,turkey,gruyere cheese,medium shrimp,");
	sort(expected);
	printf("5.4: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.5
	head = deleteList(head);
	head = related_with_restrictions("soymilk", "quinoa", 3, 4);
	strcpy(expected, "");
	sort(expected);
	printf("5.5: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.6
	head = deleteList(head);
	head = related_with_restrictions("gruyere cheese", "soymilk", 1, 4);
	strcpy(expected, "fresh breadcrumbs,");
	sort(expected);
	printf("5.6: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.7
	head = deleteList(head);
	head = related_with_restrictions("gruyere cheese", "soymilk", 1, 11);
	strcpy(expected, "");
	sort(expected);
	printf("5.7: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.8
	head = deleteList(head);
	head = related_with_restrictions("soymilk", "quinoa", 11, 1);
	strcpy(expected, "soymilk,turkey,gruyere cheese,fresh breadcrumbs,tomato juice,prepared horseradish,medium shrimp,");
	sort(expected);
	printf("5.8: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.9
	head = deleteList(head);
	head = related_with_restrictions("turkey", "soymilk", 2, 1);
	strcpy(expected, "quinoa,turkey,artichoke hearts,medium shrimp,gruyere cheese,");
	sort(expected);
	printf("5.9: %s", compare_linked_list(head, expected) == 0 ? " PASSED\n" : " FAILED\n");
	
	// 5.10
	head = deleteList(head);
	head = related_with_restrictions("turkey", "soymilk", 11, 11);
	strcpy(expected, "");
	sort(expected);
	printf("5.10: %s", compare_linked_list(head, expected) == 0 ? "PASSED\n" : "FAILED\n");
	
	// 5.11
	head = deleteList(head);
	head = related_with_restrictions("turkey", "soymilk", 11, 0);
	strcpy(expected, "sunflower seeds,quinoa,turkey,artichoke hearts,gruyere cheese,fresh breadcrumbs,tomato juice,prepared horseradish,medium shrimp,");
	sort(expected);
	printf("5.11: %s", compare_linked_list(head, expected) == 0 ? "PASSED\n" : "FAILED\n");
}

int compare_array(char actual[10][MAX_STR_LEN], const char *expected)
{
	char buffer[1024];
	buffer[0] = '\0';
	
	int rc;
	
	for (int i = 0; i < 10; i++)
	{
		strcat(buffer, actual[i]);
		strcat(buffer, ",");
	}
	sort(buffer);
	//printf("actual: %s, expected: %s\n", buffer, expected);
	return strcmp(buffer, expected);
}

void set_recipe_10(char recipe[10][MAX_STR_LEN])
{
	strcpy(recipe[0], "medium shrimp");
	strcpy(recipe[1], "prepared horseradish");
	strcpy(recipe[2], "turkey");
	strcpy(recipe[3], "soymilk");
	strcpy(recipe[4], "tomato juice");
	strcpy(recipe[5], "fresh breadcrumbs");
	strcpy(recipe[6], "artichoke hearts");
	strcpy(recipe[7], "gruyere cheese");
	strcpy(recipe[8], "sunflower seeds");
	strcpy(recipe[9], "quinoa");
}

void set_recipe_5(char recipe[10][MAX_STR_LEN])
{
	strcpy(recipe[0], "medium shrimp");
	strcpy(recipe[1], "prepared horseradish");
	strcpy(recipe[2], "turkey");
	strcpy(recipe[3], "soymilk");
	strcpy(recipe[4], "tomato juice");
	strcpy(recipe[5], "");
	strcpy(recipe[6], "");
	strcpy(recipe[7], "");
	strcpy(recipe[8], "");
	strcpy(recipe[9], "");
}

void print_recipe(char recipe[10][MAX_STR_LEN])
{
	for (int i = 0; i < 10; i++)
	{
		printf("%s\n", recipe[i]);
	}
}

void test_substitute_ingredient()
{
	// 6.1: Empty
	// 6.2: 2 ingredient recipe
	// 6.3: 5 ingredient recipe; sub soymilk
	// 6.4: 5 ingredient recipe; sub turkey
	// 6.5: 5 ingredient recipe; sub medium shrimp
	// 6.6: 5 ingredient recipe: sub tomato juice
	// 6.7: 5 ingredient recipe: sub prepared horseradish
	// 6.8: 10 ingredient recipe: sub soymilk
	// 6.9: 10 ingredient recipe: adj matrix is 0
	// 6.10: 10 ingredient recipe: sub soymilk; soymilk has the strongest weight
	
	// 6.1
	char expected[1024];
	char expected2[1024];
	char expected3[1024];
	char recipe[10][MAX_STR_LEN]={"",
								 "",
								 "",
								 "",
								 "",
								 "",
								 "",
								 "",
								 "",
								 ""};
	ghetto_load_ingredients();
	substitute_ingredient(recipe,"tomato juice");
	strcpy(expected, ",,,,,,,,,,");
	sort(expected);
	printf("6.1: %s", compare_array(recipe, expected) == 0 ? "PASSED\n" : "FAILED\n");
	
	// 6.2
	strcpy(recipe[0], "medium shrimp");
	strcpy(recipe[1], "prepared horseradish");
	
	substitute_ingredient(recipe,"prepared horseradish");
	strcpy(expected, "medium shrimp,artichoke hearts,,,,,,,,,");
	sort(expected);
	printf("6.2: %s", compare_array(recipe, expected) == 0 ? "PASSED\n" : "FAILED\n");
	//print_recipe(recipe);
	
	// 6.3
	set_recipe_5(recipe);
	substitute_ingredient(recipe,"soymilk");
	strcpy(expected, "prepared horseradish,medium shrimp,tomato juice,turkey,artichoke hearts,,,,,,");
	strcpy(expected2, "prepared horseradish,medium shrimp,tomato juice,turkey,fresh breadcrumbs,,,,,,");
	sort(expected);
	sort(expected2);
	printf("6.3: %s", (compare_array(recipe, expected) == 0) || (compare_array(recipe, expected2) == 0) ? "PASSED\n" : "FAILED\n");
	//print_recipe(recipe);
	
	// 6.4
	set_recipe_5(recipe);
	substitute_ingredient(recipe,"turkey");
	strcpy(expected, "prepared horseradish,medium shrimp,tomato juice,fresh breadcrumbs,soymilk,,,,,,");
	strcpy(expected2, "prepared horseradish,medium shrimp,tomato juice,artichoke hearts,soymilk,,,,,,");
	strcpy(expected3, "prepared horseradish,medium shrimp,tomato juice,sunflower seeds,soymilk,,,,,,");
	sort(expected);
	sort(expected2);
	sort(expected3);
	printf("6.4: %s", (compare_array(recipe, expected) == 0) || (compare_array(recipe, expected2)) == 0 || ((compare_array(recipe, expected3)))? "PASSED\n" : "FAILED\n");
	
	// 6.5
	set_recipe_5(recipe);
	substitute_ingredient(recipe,"medium shrimp");
	strcpy(expected, "soymilk,turkey,fresh breadcrumbs,tomato juice,prepared horseradish,,,,,,");
	strcpy(expected2, "soymilk,turkey,sunflower seeds,tomato juice,prepared horseradish,,,,,,");
	sort(expected);
	sort(expected2);
	printf("6.5: %s", (compare_array(recipe, expected) == 0) || (compare_array(recipe, expected2)) == 0? "PASSED\n" : "FAILED\n");
	//print_recipe(recipe);
	
	// 6.6
	set_recipe_5(recipe);
	substitute_ingredient(recipe,"tomato juice");
	strcpy(expected, "soymilk,turkey,medium shrimp,artichoke hearts,prepared horseradish,,,,,,");
	strcpy(expected2, "soymilk,turkey,medium shrimp,sunflower seeds,prepared horseradish,,,,,,");
	sort(expected);
	sort(expected2);
	printf("6.6: %s", (compare_array(recipe, expected) == 0) || (compare_array(recipe, expected2)) == 0? "PASSED\n" : "FAILED\n");
	//print_recipe(recipe);
	
	// 6.7
	set_recipe_5(recipe);
	substitute_ingredient(recipe,"prepared horseradish");
	strcpy(expected, "soymilk,turkey,medium shrimp,artichoke hearts,tomato juice,,,,,,");
	strcpy(expected2, "soymilk,turkey,medium shrimp,fresh breadcrumbs,tomato juice,,,,,,");
	strcpy(expected3, "soymilk,turkey,medium shrimp,sunflower seeds,tomato juice,,,,,,");
	sort(expected);
	sort(expected2);
	sort(expected3);
	printf("6.7: %s", (compare_array(recipe, expected) == 0) || (compare_array(recipe, expected2)) == 0 || ((compare_array(recipe, expected3)))? "PASSED\n" : "FAILED\n");
	
	// 6.8
	set_recipe_10(recipe);
	substitute_ingredient(recipe,"soymilk");
	strcpy(expected, "medium shrimp,prepared horseradish,turkey,tomato juice,fresh breadcrumbs,artichoke hearts,gruyere cheese,sunflower seeds,quinoa,,");
	sort(expected);
	printf("6.8: %s", (compare_array(recipe, expected) == 0)? "PASSED\n" : "FAILED\n");
	//print_recipe(recipe);
	
	// 6.9
	set_recipe_10(recipe);
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			AdjMat[i][j] = 0;
		}
	}
	
	substitute_ingredient(recipe,"soymilk");
	strcpy(expected, "medium shrimp,prepared horseradish,turkey,tomato juice,fresh breadcrumbs,artichoke hearts,gruyere cheese,sunflower seeds,quinoa,,");
	sort(expected);
	printf("6.9: %s", (compare_array(recipe, expected) == 0)? "PASSED\n" : "FAILED\n");
	
	// 6.10
	set_recipe_10(recipe);
	for (int i = 0; i < 10; i++)
	{
		AdjMat[3][i] = 10;
	}
	
	substitute_ingredient(recipe,"soymilk");
	strcpy(expected, "medium shrimp,prepared horseradish,turkey,tomato juice,fresh breadcrumbs,artichoke hearts,gruyere cheese,sunflower seeds,quinoa,,");
	sort(expected);
	printf("6.10: %s", (compare_array(recipe, expected) == 0)? "PASSED\n" : "FAILED\n");
	//print_recipe(recipe);

}

int main()
{
	//test_print_ingredients();
	//test_ingredient_index();
	//test_related_ingredients();
	//test_k_dist();
	//test_related_with_restrictions();
	//test_substitute_ingredient();
	//print_adj();
}
