/* CREATED BY STESTEIN, the infamous harass'her
	IN COLLABORATION WITH: ... */

// Current goal: get analysis to properly identify past motive waves
// Once proper, implement live stream of data and live updating of the wave beased on input

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

	// Attempt to find Impulse Motive Wave Where:
	// Point 2 cannot be < start
	// Point 4 cannot be < Point 1
	// Wave 3 Cannot be shortest length (see Diagram 1)

char **pattern_seeker(char **data_array)
{
	return (data_array);
}				

// will search through CSV file and designate what index the data points are at
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

char 	**the_gatherer(char **data_array, t_index *index)
{
	// designed to calculate from hourly open/close values
	double open_price = 0;
	double close_price = 0;
	int row = 1;
	int x = 0;
	int open_pos;
	int close_pos;
	char *value = 0;
	int	i = 0;
	char	*date;
	char	*open;
	char	*close;
	char	**calc_data;
	int 	col = 0;

	calc_data = (char **)malloc(sizeof(char) * 10000);
	open = (char *)malloc(sizeof(char) * 20);
	close = (char *)malloc(sizeof(char) * 20);
	date = (char *)malloc(sizeof(char) * 20);
	while (data_array[row])
	{
		x = 0;
		open_pos = index->open;
		close_pos = index->close;
		calc_data[col] = (char *)malloc(sizeof(char) * 20);
		// gets date and time of the current data
		while (data_array[row][x] != ',')
		{
			date[i] = data_array[row][x];
			i++;
			x++;
		}
		x = 0;
		date[i] = '\0';
		calc_data[col] = date;
		col++;
		calc_data[col] = (char *)malloc(sizeof(char) * 20);
		// gets open price of the hour	
		i = 0;
		while (open_pos > 0)
		{
			while (data_array[row][x] != ',')
				x++;
			x++;
			open_pos--;
		}
		while (data_array[row][x] != ',')
		{
			open[i] = data_array[row][x];
			i++;
			x++;
		}
		open[i] = '\0';
		//open_price = atof(open);
		calc_data[col] = open;
		col++;
		calc_data[col] = (char *)malloc(sizeof(char) * 20);
		x = 0;
		i = 0;
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
			close[i] = data_array[row][x];
			i++;
			x++;
		}
		// row is now also the line # of the result, can be used for indexing
		close[i] = '\0';
		calc_data[col] = close;
		col++;
		//close_price = atof(open);
		i = 0;	
		
		// tests if correctly grabbed open/close from data_array
		
		/*printf("date: %s\nopen: %s\nclose: %s\n", date, open, close);
		printf("--------------------------------------\n");
		printf("%s, %s, %s\n", calc_data[0], calc_data[1], calc_data[2]);*/
		row++;
	}
	i = 0;
	while (calc_data[i])
	{
		printf("%s\n", calc_data[i]);
		i++;
	}
	return (data_array);
}
	
		
			
			
			
		
			

	

	

char	**pattern_detector(char **data_array, t_index *index)
{
	char	**calc_data;
	//	other_checks_go_here = 0;
	
	calc_data = the_gatherer(data_array, index);
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
	

	// For testing initial 2d data structure

	/* while (data_array[row])
	{
		printf("%s", data_array[row]);
		row++;
	} */

	// Tests if index struct is filling indexes properly
	
	/* printf("date: %d\n", index.date);
	printf("open: %d\n", index.open);
	printf("high: %d\n", index.high);
	printf("low: %d\n", index.low);
	printf("close: %d\n", index.close);
	printf("volume: %d\n", index.volume);
	patterns = pattern_seeker(data_array); */
	
}	
