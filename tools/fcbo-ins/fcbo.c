/*  FCbO: Fast Close-by-One (INS Version; http://fcalgs.sourceforge.net/)
 *
 *  Copyright (C) 2007-2010
 *  Jan Outrata, <jan.outrata@upol.cz>
 *  Vilem Vychodil, <vilem.vychodil@upol.cz>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define BIT		((unsigned long) 1)
#define NULL_LONG	((unsigned long) 0)
#define INT_SIZE	(sizeof (int))
#define LONG_SIZE	(sizeof (unsigned long))
#define ARCHBIT		((LONG_SIZE * 8) - 1)
#define BYTE_COUNT_A	(LONG_SIZE * int_count_a)
#define BYTE_COUNT_O	(LONG_SIZE * int_count_o)
#define BUFFER_BLOCK	1024

int attributes = 0;
int objects = 0;
int int_count_a = 0;
int int_count_o = 0;
int table_entries = 0;
int min_support = 0;
unsigned long *context;
unsigned long **cols;
int *supps;
int *attrib_numbers;
unsigned long upto_bit[ARCHBIT + 1];
int attr_offset = 0;
FILE *in_file;
FILE *out_file;

int verbosity_level = 1;
struct stats_t
{ int total; int closures; int fail_canon; int fail_fcbo; int fail_support; } stats = {0, 0, 0, 0, 0};
struct timeval time_start, time_inner, time_end;
int *buff = NULL;
int buff_index = 0;
size_t buff_size = BUFFER_BLOCK;

int
get_next_integer(file, value)
     FILE *file;
     int *value;
{
  int ch = ' ';
  *value = -1;
  while((ch != EOF) && ((ch < '0') || (ch > '9')))
    {
      ch = fgetc(file);
      if(ch == '\n')
	return 1;
    }
  if(ch == EOF)
    return 0;
  *value = 0;
  while((ch >= '0') && (ch <= '9'))
    {
      *value *= 10;
      *value += ch - '0';
      ch = fgetc(file);
    }
  ungetc(ch, file);
  *value -= attr_offset;
  return 1;
}

void
allocate_buffer(buffer, size)
     int **buffer;
     int size;
{
  if(*buffer)
    *buffer = (int *)realloc(*buffer, INT_SIZE * size);
  else
    *buffer = (int *)malloc(INT_SIZE * size);
  if(!*buffer)
    {
      fprintf(stderr, "Cannot reallocate buffer, quitting.");
      exit(3);
    }
}

#define PUSH_NEW_INTEGER(__value) \
  { \
	if (buff_index >= buff_size) { \
      buff_size += BUFFER_BLOCK; \
      allocate_buffer (&buff, buff_size); \
	} \
	buff [buff_index] = (__value); \
	buff_index ++; \
  }

void
read_file(file)
     FILE *file;
{
  int last_value = -1, value = 0, last_attribute = -1, last_object = -1;
  allocate_buffer(&buff, buff_size);
  while(get_next_integer(file, &value))
    {
      if((value < 0) && (last_value < 0))
	continue;
      if(value < 0)
	{
	  last_object++;
	  PUSH_NEW_INTEGER(-1);
	}
      else
	{
	  if(value > last_attribute)
	    last_attribute = value;
	  PUSH_NEW_INTEGER(value);
	}
      last_value = value;
    }
  if(last_value >= 0)
    {
      last_object++;
      PUSH_NEW_INTEGER(-1);
    }
  objects = last_object + 1;
  attributes = last_attribute + 1;
}

void
create_context(void)
{
  int i = 0, row = 0;
  int_count_a = (attributes / (ARCHBIT + 1)) + 1;
  int_count_o = (objects / (ARCHBIT + 1)) + 1;
  context = (unsigned long *)malloc(LONG_SIZE * int_count_a * objects);
  if(!context)
    {
      fprintf(stderr, "Cannot allocate bitcontext, quitting.");
      exit(5);
    }
  memset(context, 0, LONG_SIZE * int_count_a * objects);
  supps = (int *)malloc(sizeof(int) * attributes);
  memset(supps, 0, sizeof(int) * attributes);
  for(i = 0; i < buff_index; i++)
    {
      if(buff[i] < 0)
	{
	  row++;
	  continue;
	}
      context[row * int_count_a + (buff[i] / (ARCHBIT + 1))] |= (BIT << (ARCHBIT - (buff[i] % (ARCHBIT + 1))));
      supps[buff[i]]++;
      table_entries++;
    }
  if(verbosity_level >= 2)
    fprintf(stderr, "objects: %6i\nattributes: %4i\nentries: %8i\n", objects, attributes, table_entries);
}

void
initialize_output(void)
{
  int i;
  attrib_numbers = (int *)malloc(sizeof(int) * attributes);
  for(i = 0; i < attributes; i++)
    attrib_numbers[i] = i;
}

void
print_attributes(set, supp)
     unsigned long *set;
     size_t supp;
{
  int i, j, c;
  int first = 1;
  if(verbosity_level <= 0)
    return;
  for(c = j = 0; j < int_count_a; j++)
    {
      for(i = ARCHBIT; i >= 0; i--)
	{
	  if(set[j] & (BIT << i))
	    {
	      if(!first)
		fprintf(out_file, " ");
	      fprintf(out_file, "%i", attrib_numbers[c]);
	      first = 0;
	    }
	  c++;
	  if(c >= attributes)
	    goto out;
	}
    }
out:
  fprintf(out_file, "\n");
}

int
cols_compar(a, b)
     void *a, *b;
{
  int x, y;
  x = supps[*(int const *)a];
  y = supps[*(int const *)b];
  if(x >= min_support)
    {
      if(y >= min_support)
	return (x < y) ? -1 : ((x > y) ? 1 : 0);
      else
	return -1;
    }
  else
    {
      if(y >= min_support)
	return 1;
      else
	return (x < y) ? -1 : ((x > y) ? 1 : 0);
    }
}

int
rows_compar(a, b)
     void *a, *b;
{
  int i;
  for(i = 0; i < int_count_a; i++)
    if(((unsigned long *)a)[i] < ((unsigned long *)b)[i])
      return -1;
    else if(((unsigned long *)a)[i] > ((unsigned long *)b)[i])
      return 1;
  return 0;
}

void
sort_context(void)
{
  int i, j, k, x, y, z, ii, jj, a, aa;
  unsigned long *new_context;

  for(a = i = 0; i < attributes; i++)
    if(supps[i] >= min_support)
      a++;

  qsort(attrib_numbers, attributes, sizeof(int), cols_compar);
  aa = attributes;
  attributes = a;
  a = int_count_a;
  int_count_a = (attributes / (ARCHBIT + 1)) + 1;
  new_context = (unsigned long *)malloc(LONG_SIZE * int_count_a * objects);
  memset(new_context, 0, LONG_SIZE * int_count_a * objects);
  for(k = jj = 0, ii = ARCHBIT; k < aa; k++)
    {
      if(supps[attrib_numbers[k]] < min_support)
	continue;
      j = attrib_numbers[k] / (ARCHBIT + 1);
      i = ARCHBIT - (attrib_numbers[k] % (ARCHBIT + 1));
      for(x = 0, y = j, z = jj; x < objects; x++, y += a, z += int_count_a)
	if(context[y] & (BIT << i))
	  new_context[z] |= (BIT << ii);
      if(ii > 0)
	ii--;
      else
	{
	  ii = ARCHBIT;
	  jj++;
	}
    }
  free(context);
  context = new_context;
  qsort(context, objects, BYTE_COUNT_A, rows_compar);
}

void
initialize_algorithm(void)
{
  int i, j, k, x, y;
  unsigned long *ptr, mask, *cols_buff;
  for(i = 0; i <= ARCHBIT; i++)
    {
      upto_bit[i] = NULL_LONG;
      for(j = ARCHBIT; j > i; j--)
	upto_bit[i] |= (BIT << j);
    }
  cols_buff = (unsigned long *)malloc(LONG_SIZE * int_count_o * attributes);
  memset(cols_buff, 0, LONG_SIZE * int_count_o * attributes);
  cols = (unsigned long **)malloc(sizeof(unsigned long *) * attributes);
  ptr = cols_buff;
  for(k = j = 0; j < int_count_a; j++)
    for(i = ARCHBIT; i >= 0; i--, k++)
      {
	if(k >= attributes)
	  return;
	mask = (BIT << i);
	cols[k] = ptr;
	for(x = 0, y = j; x < objects; x++, y += int_count_a)
	  if(context[y] & mask)
	    ptr[x / (ARCHBIT + 1)] |= BIT << (x % (ARCHBIT + 1));
	ptr += int_count_o;
      }
}

void
compute_closure(intent, extent, prev_extent, attr_extent, supp)
     unsigned long *intent, *extent, *prev_extent, *attr_extent;
     int *supp;
{
  int i, j, k, l;
  stats.closures++;
  memset(intent, 0xFF, BYTE_COUNT_A);
  if(attr_extent)
    {
      *supp = 0;
      for(k = 0; k < int_count_o; k++)
	{
	  extent[k] = prev_extent[k] & attr_extent[k];
	  if(extent[k])
	    for(l = 0; l <= ARCHBIT; l++)
	      {
		if(extent[k] >> l)
		  {
		    if((extent[k] >> l) & BIT)
		      {
			(*supp)++;
			for(i = 0, j = int_count_a * (k * (ARCHBIT + 1) + l); i < int_count_a; i++, j++)
			  intent[i] &= context[j];
		      }
		  }
		else
		  break;
	      }
	}
    }
  else
    {
      memset(extent, 0xFF, BYTE_COUNT_O);
      for(j = 0; j < objects; j++)
	{
	  for(i = 0; i < int_count_a; i++)
	    intent[i] &= context[int_count_a * j + i];
	}
    }
}

void
generate_from_node(intent, extent, start_int, start_bit, starts, implied, implied_stack)
     unsigned long *intent;
     unsigned long *extent;
     int start_int, start_bit;
     int *starts;
     unsigned long **implied;
     unsigned long ***implied_stack;
{
  int i, total, supp = 0;
  unsigned long *new_extent, *new_intent, *new_intent_i, *new_intents_head;
  unsigned long ***implied_stack_i = implied_stack;
  int *start_i = starts;
  total = start_int * (ARCHBIT + 1) + (ARCHBIT - start_bit);
  new_intent = new_intent_i = new_intents_head = (unsigned long *)malloc((BYTE_COUNT_A + BYTE_COUNT_O) * (attributes - total));
  new_extent = new_intent + int_count_a;
  for(; start_int < int_count_a; start_int++)
    {
      for(; start_bit >= 0; start_bit--)
	{
	  if(total >= attributes)
	    goto skipout;
	  if(intent[start_int] & (BIT << start_bit))
	    goto skip;
	  if(implied[total] != NULL)
	    {
	      if(implied[total][start_int] & ~(intent[start_int]) & upto_bit[start_bit])
		{
		  stats.fail_fcbo++;
		  goto skip;
		}
	      for(i = 0; i < start_int; i++)
		if(implied[total][i] & ~(intent[i]))
		  {
		    stats.fail_fcbo++;
		    goto skip;
		  }
	    }
	  compute_closure(new_intent, new_extent, extent, cols[total], &supp);
	  if(supp < min_support)
	    {
	      stats.fail_support++;
	      goto skip;
	    }
	  if((new_intent[start_int] ^ intent[start_int]) & upto_bit[start_bit])
	    {
	      stats.fail_canon++;
	      goto skiptoelse;
	    }
	  for(i = 0; i < start_int; i++)
	    if(new_intent[i] ^ intent[i])
	      {
		stats.fail_canon++;
		goto skiptoelse;
	      }
	  print_attributes(new_intent, supp);
	  stats.total++; *start_i = start_int; start_i++;
	  *start_i = start_bit; start_i++;
	  goto skipoverelse;
	skiptoelse:
	  *implied_stack_i = &(implied[total]); implied_stack_i++;
	  *implied_stack_i = (unsigned long **)implied[total]; implied_stack_i++;
	  implied[total] = new_intent;
	  new_intent[int_count_a - 1] |= BIT;
	skipoverelse:
	  new_intent = new_extent + int_count_o;
	  new_extent = new_intent + int_count_a;
	skip:
	  total++;
	}
      start_bit = ARCHBIT;
    }
skipout:
  for(; new_intent_i != new_intent; new_intent_i = new_extent + int_count_o)
    {
      new_extent = new_intent_i + int_count_a;
      if(new_intent_i[int_count_a - 1] & BIT)
	continue;
      if(*(starts + 1) == 0)
	generate_from_node(new_intent_i, new_extent, *starts + 1, ARCHBIT, start_i, implied, implied_stack_i);
      else
	generate_from_node(new_intent_i, new_extent, *starts, *(starts + 1) - 1, start_i, implied, implied_stack_i);
      starts += 2;
    }
  for(; implied_stack != implied_stack_i; implied_stack += 2)
    **implied_stack = (unsigned long *)*(implied_stack + 1);
  free(new_intents_head);
  return;
}

void
find_all_intents(void)
{
  unsigned long *extent;
  unsigned long *intent;
  int *starts;
  unsigned long **implied;
  unsigned long ***implied_stack;
  intent = (unsigned long *)malloc(BYTE_COUNT_A + BYTE_COUNT_O);
  extent = intent + int_count_a;
  compute_closure(intent, extent, NULL, NULL);
  print_attributes(intent, objects);
  stats.total++;
  if(intent[int_count_a - 1] & BIT)
    return;
  starts = (int *)malloc(sizeof(int) * (attributes + 1) * attributes);
  implied = (unsigned long **)malloc(sizeof(unsigned long *) * attributes);
  memset(implied, 0, sizeof(unsigned long *) * attributes);
  implied_stack = (unsigned long ***)malloc(sizeof(unsigned long **) * (attributes + 1) * attributes);
  generate_from_node(intent, extent, 0, ARCHBIT, starts, implied, implied_stack);
}

int
main(argc, argv)
     int argc;
     char **argv;
{
  in_file = stdin;
  out_file = stdout;
  if(argc > 1)
    {
      int index = 1;
      for(; (index < argc && argv[index][0] == '-' && argv[index][1] != 0); index++)
	{
	  switch (argv[index][1])
	    {
	    case 'S':
	      min_support = atoi(argv[index] + 2);
	      break;
	    case 'V':
	      verbosity_level = atoi(argv[index] + 2);
	      break;
	    case 'h':
	      fprintf(stderr, "synopsis: %s [-h] [-index] [-Smin-support] [-Vlevel] [INPUT-FILE] [OUTPUT-FILE]\n", argv[0]);
	      return 0;
	    default:
	      attr_offset = atoi(argv[index] + 1);
	      if(attr_offset < 0)
		attr_offset = 0;
	    }
	}
      if((argc > index) && (argv[index][0] != '-'))
	in_file = fopen(argv[index], "rb");
      if((argc > index + 1) && (argv[index + 1][0] != '-'))
	out_file = fopen(argv[index + 1], "wb");
    }
  if(!in_file)
    {
      fprintf(stderr, "%s: cannot open input data stream\n", argv[0]);
      return 1;
    }
  if(!out_file)
    {
      fprintf(stderr, "%s: open output data stream\n", argv[0]);
      return 2;
    }
  if(verbosity_level >= 3)
    gettimeofday(&time_start, NULL);
  read_file(in_file);
  create_context();
  free(buff);
  fclose(in_file);
  if(verbosity_level >= 3)
    gettimeofday(&time_inner, NULL);
  initialize_output();
  sort_context();
  initialize_algorithm();
  find_all_intents();
  if(verbosity_level >= 3)
    {
      gettimeofday(&time_end, NULL);
      {
	long int usec_less = (time_end.tv_usec < time_inner.tv_usec) ? 1 : 0;
	fprintf(stderr, "inner time: %li.%06li s\n", time_end.tv_sec - time_inner.tv_sec - usec_less, usec_less * 1000000 + time_end.tv_usec - time_inner.tv_usec);
	usec_less = (time_end.tv_usec < time_start.tv_usec) ? 1 : 0;
	fprintf(stderr, "total time: %li.%06li s\n", time_end.tv_sec - time_start.tv_sec - usec_less, usec_less * 1000000 + time_end.tv_usec - time_start.tv_usec);
      }
    }
  if(verbosity_level >= 2)
    fprintf(stderr, "total: %i\nclosures: %i\nfail_canon: %i\nfail_fcbo: %i\nfail_support: %i\n", stats.total, stats.closures, stats.fail_canon, stats.fail_fcbo, stats.fail_support);
  fclose(out_file);
  return 0;
}
