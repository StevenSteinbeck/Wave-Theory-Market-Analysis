/* CREATED BY STESTEIN, the infamous harass'her
	IN COLLABORATION WITH: ... */

// Current goal: get analysis to properly identify past motive waves
// Once proper, implement live stream of data and live updating/prediction of the live wave beased on input
// Then include an automated tradng bot based on confidence levels of current position in motive wave

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void	ft_strclr(char *str);

// store ints to reference their pos in array
typedef	struct	s_index
{
	int	date;
	int	open;
	int	high;
	int	low;
	int	close;
	int	volume;
}		t_index;

// gathers data from CSV file
// eventually replace with live data stream
char	*getdata(void)
{
	char *buffer = 0;
	long length;
	char *str;
	char *name;
	int i;
	FILE * f = fopen ("BTC_DATA.csv", "r");
	
	if (f)
	{
  		fseek (f, 0, SEEK_END);
  		length = ftell (f);
  		fseek (f, 0, SEEK_SET);
  		buffer = malloc (length);
  		if (buffer)
    			fread (buffer, 1, length, f);
  		fclose (f);
	}
	return (buffer);		
}

// converts initial string read from CSV file to a more searchable 2D array
char	**oned_twod(char *data, char **data_array)
{
	int i = 0;
	int x = 0;
	int row = 0;

	data_array = (char **)malloc(sizeof(char *) * 10000);
	while (data[i])
	{
		x = 0;
		data_array[row] = (char *)malloc(sizeof(char) * 300);
		while (data[i] != '\n' && data[i])
		{	
			data_array[row][x] = data[i];
			i++;
			x++;
		}
		data_array[row][x] = '\n';
		data_array[row][x + 1] = '\0';
		row++;
		if (data[i] == '\n' && data[i + 2] == ',')
		{
			i++;
			while (data[i] != '\n')
				i++;
		}
		if (data[i] == '\0')
			break ;
		i++;
	}
	data_array[row] = NULL;
	return (data_array);
}

// will smart search through CSV file and designate what index the data points are at
void	index_setup(char **data_array, t_index *index)
{
	int	row = 0;
	int	i = 0;
	int	z = 0;
	int	curr_index = 0;
	char	*labels;
	char	*word;

	labels = data_array[row];
	while (labels[i])
	{
		z = 0;
		word = (char *)malloc(sizeof(char) * 50);
		while (labels[i] != ',' && labels[i] && labels[i] >= 65
			&& labels[i] <= 122)
		{
			word[z] = labels[i];
			i++;
			z++;	
		}
		if ((strcmp(word, "Date") == 0))
			index->date = curr_index;
		if ((strcmp(word, "Open") == 0))
			index->open = curr_index;
		if ((strcmp(word, "High") == 0))
			index->high = curr_index;
		if ((strcmp(word, "Low") == 0))
			index->low = curr_index;
		if ((strcmp(word, "Close") == 0))
			index->close = curr_index;
		if ((strcmp(word, "Volume") == 0))
			index->volume = curr_index;
		curr_index++;
		i++;
		ft_strclr(word);
	}
}

void	ft_strclr(char *s)
{
	int i;

	i = 0;
	if (s == NULL)
		return ;
	while (s[i])
	{
		s[i] = '\0';
		i++;
	}
}

// gets hourly open/close values and associated timestamp
char	**the_gatherer(char **data_array, t_index *index)
{
	int row = 1;
	int x = 0;
	int open_pos;
	int close_pos;
	int 	open_copy = index->open;
	int	close_copy = index->close;
	char	**calc_data;
	int 	j = 0;
	int 	col = 0;

	calc_data = (char **)malloc(sizeof(char *) * 20000);
	while (data_array[row])
	{
		j = 0;
		x = 0;
		open_pos = open_copy;
		close_pos = close_copy;
		// gets date and time of the current data
		calc_data[col] = (char *)malloc(sizeof(char) * 30);
		while (data_array[row][x] != ',')
		{
			calc_data[col][j] = data_array[row][x];
			j++;
			x++;
		}
		x++;
		calc_data[col][j] = '\0';
		col++;
		calc_data[col] = (char *)malloc(sizeof(char) * 30);
		// gets open price of the hour	
		x = 0;
		j = 0;
		while (open_pos > 0)
		{
			while (data_array[row][x] != ',')
				x++;
			x++;
			open_pos--;
		}
		while (data_array[row][x] != ',')
		{
			calc_data[col][j] = data_array[row][x];
			j++;
			x++;
		}
		x++;
		calc_data[col][j] = '\0';
		col++;
		calc_data[col] = (char *)malloc(sizeof(char) * 30);
		x = 0;
		j = 0;
		// gets close price of the hour
		while (close_pos > 0)
		{
			while (data_array[row][x] != ',')
				x++;
			x++;
			close_pos--;
		}
		while (data_array[row][x] != ',')
		{
			calc_data[col][j] = data_array[row][x];
			j++;
			x++;
		}
		calc_data[col][j] = '\0';
		col++;	
		row++;
	}
	calc_data[col] = NULL;
	col = 0;
	return (calc_data);
}

double	get_base_val(char **calc_data, int *row)
{
	int i = 0;
	char	*base_val;
	double	base = 0;

	base_val = (char *)malloc(sizeof(char) * 20);
	if (calc_data[*row][1] == '/' || calc_data[*row][2] == '/')
		(*row)++;
	while (calc_data[*row][i])
	{
		base_val[i] = calc_data[*row][i];
		i++;
	}
	base = atof(base_val);
	free(base_val);
	return (base);
}

double	get_next_val(char **calc_data, int *row, int interval)
{
	int i = 0;
	char	*val;
	double	val1 = 0;
	int	row_cpy = 0;
	
	if (calc_data[*row + interval] != 0)
		*row = *row + interval;
	row_cpy = *row;
	val = (char *)malloc(sizeof(char) * 20);
	if (calc_data[*row][1] == '/' || calc_data[*row][2] == '/')
		(*row)++;
	while (calc_data[*row][i])
	{
		val[i] = calc_data[*row][i];
		i++;
	}
	val1 = atof(val);
	free(val);
	return (val1);
}

char	*date_getter(char **calc_data, int row)
{
	int	i = 0;
	int	x = 0;
	char	*date;
	
	date = (char *)malloc(sizeof(char) * 30);
	if (calc_data[row][1] == '/' || calc_data[row][2] == '/')
	{
		while (calc_data[row][i])
		{
			date[i] = calc_data[row][i];
			i++;
		}
		return (date);
	}
	--row;
	if (calc_data[row] != 0 && (calc_data[row][1] == '/' || calc_data[row][2] == '/'))
	{
		while (calc_data[row][i])
		{
			date[i] = calc_data[row][i];
			i++;
		}
		return (date);
	}
	row = row + 2;
	//trouble with incrementation making the '/' check, causing seg fault, rehtink
	if (calc_data[row] == NULL)
		x++;
	if (x == 0)
	{	
		if (calc_data[row][1] == '/' || calc_data[row][2] == '/')
		{
			while (calc_data[row][i])
			{
				date[i] = calc_data[row][i];
				i++;
			}
			return (date);	
			
		}
	}
	return ("poop");
}

void	itoa_isnegative(int *n, int *negative)
{
	if (*n < 0)
	{
		*n *= -1;
		*negative = 1;
	}
}

char	*ft_itoa(int n)
{
	int		tmpn;
	int		len;
	int		negative;
	char	*str;

	tmpn = n;
	len = 2;
	negative = 0;
	itoa_isnegative(&n, &negative);
	while (tmpn /= 10)
		len++;
	len += negative;
	if ((str = (char*)malloc(sizeof(char) * len)) == NULL)
		return (NULL);
	str[--len] = '\0';
	while (len--)
	{
		str[len] = n % 10 + '0';
		n = n / 10;
	}
	if (negative)
		str[0] = '-';
	return (str);
}

char	**motive_impulse_engine(char **calc_data)
{
	int 	row_start = 0;
	int 	row_end = 0;
	int 	row = 0;
	int	interval = 1;
	double	val_0;
	double	val_1;
	double	val_2;
	double	val_3;
	double	val_4;
	double	val_5;
	double	wave_1;
	double	wave_2;
	double	wave_3;
	double	wave_4;
	double	wave_5;
	char	**matches;
	int	col = 0;
	int	x = 0;
	char	*date;
	char	*date_1;
	char	*date_2;
	char	*date_3;
	char	*date_4;
	char	*date_5;

	matches = (char **)malloc(sizeof(char *) * 12000);
	
	// (1) add date/time output printing instead of row #'s
	// (2) output data needs to be more readable / analytic
	// (3) add support for imperfect matches and ouput a confidence level in a match instead

	// this loop changes the search interval so it will find every match possible over a short and eventually long period of time
	while (interval < 1000)
	{
		row = 0;
		// this loop searches through every price (start to end) with each time interval, starting with 1 hour intervals
		while (calc_data[row])
		{
			date = date_getter(calc_data, row);	
			row_start = row;	
			val_0 = get_base_val(calc_data, &row);
			val_1 = get_next_val(calc_data, &row, interval);
			wave_1 = val_1 - val_0;
			if (val_0 < val_1)
			{
				date_1 = date_getter(calc_data, row);
				wave_2 = val_1 - val_2;
				val_2 = get_next_val(calc_data, &row, interval);
				if (val_2 < val_1 && val_2 > val_0)
				{
					date_2 = date_getter(calc_data, row);
					val_3 = get_next_val(calc_data, &row, interval);
					wave_3 = val_3 - val_2;
					if (val_3 > val_2 && val_3 > val_1)
					{
						date_3 = date_getter(calc_data, row);
						val_4 = get_next_val(calc_data, &row, interval);
						wave_4 = val_3 - val_4;
						if (val_4 < val_3 && val_4 > val_1)
						{
							date_4 = date_getter(calc_data, row);
							val_5 = get_next_val(calc_data, &row, interval);
							date_5 = date_getter(calc_data, row);
							wave_5 = val_5 - val_4;
							row_end = row;
							if (val_5 > val_4 && (wave_3 > wave_1 || wave_3 > wave_2 || wave_3 > wave_4
								|| wave_3 > wave_5))
							{
								// perfect motive impulse wave match if we get here
								// store these values for graphing later
								matches[col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = ft_itoa(interval);	
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = date;
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								gcvt(val_0, 7, matches[col]);
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = date_1;
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								gcvt(val_1, 7, matches[col]);
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = date_2;
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								gcvt(val_2, 7, matches[col]);
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = date_3;
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								gcvt(val_3, 7, matches[col]);
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = date_4;
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								gcvt(val_4, 7, matches[col]);
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = date_5;
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								gcvt(val_5, 7, matches[col]);
								matches[++col] = (char *)malloc(sizeof(char) * 30);
								matches[col] = "----------";
								col++;
								/*printf("Motive Impulse Wave Detected From: Row %i %6.2f to Row %i %6.2f\n", 
									row_start, val_0, row_end, val_5);
								printf("Interval Size in Hours: %i\n", interval);*/
							}
						}
					}
				}
			}
			row++;
		}
		interval++;
	}
	return (matches);
}	

// control funciton for detecting patterns in data
// need to add support for other types of detectable waves in future	
char	**pattern_detector(char **data_array, t_index *index)
{
	char	**calc_data;
	char	**matches;
	int i = 0;
	char *copy;
	
	
	calc_data = the_gatherer(data_array, index);
	matches = motive_impulse_engine(calc_data);
	//match_prices = get_prices(matches);
	if (matches[i] != 0)
		printf("%s\n", "MOTIVE IMPULSE WAVE DETECTED");
	while (matches[i])
	{
		copy = matches[i];
		printf("%s\n", matches[i]);
		i++;
	}	
	return (data_array);
}		

int	main(void)
{
	char	*data = NULL;
	char	**data_array;
	char	**patterns;
	int	x = 0;
	int	row = 0;
	t_index	index;

	data = getdata();
	data_array = oned_twod(data, data_array);
	index_setup(data_array, &index);
	patterns = pattern_detector(data_array, &index);	
	

	// Tests if captured 2d data structure properly

	/*while (data_array[row])
	{
		printf("%s", data_array[row]);
		row++;
	}*/

	// Tests if index struct is filling properly
	/*	
	printf("date: %d\n", index.date);
	printf("open: %d\n", index.open);
	printf("high: %d\n", index.high);
	printf("low: %d\n", index.low);
	printf("close: %d\n", index.close);
	printf("volume: %d\n", index.volume);*/
	
}	
