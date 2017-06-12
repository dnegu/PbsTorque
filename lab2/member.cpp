int Member(int value)
{
	struct  list_node_s* temp_p;

	pthread_mutex_lock(&head_p_mutex);
	temp_p = head_p;
	while(temp_p!=NULL && temp_p->data < value){
		if(temp_p->next != NULL)	pthread_mutex_lock(&(temp_p->next->mutex));
		if(temp_p == head_p)	pthread_mutex_unlock(&head_p_mutex);

		pthread_mutex_unlock(&(temp_p->mutex));
		temp_p = temp_p->next;
	}

	if(temp_p==NULL || temp_p->data>value){
		if(temp_p==head_p)	pthread_mutex_unlock(&head_p_mutex);
		if(temp_p!=NULL)	pthread_mutex_unlock(&(temp_p->mutex));
		return 0;
	}
	else{
		if(temp_p==head_p)	pthread_mutex_unlock(&head_p_mutex);
		pthread_mutex_unlock(&(temp_p->mutex));
		return 1;
	}
}

void *Pth_mat_vect(void* rank){
	long my_rank = (long)rank;
	int i,j;
	int local_m = m/thread_count;
	int my_first_row = my_rank*local_m;
	int my_last_row = (my_rank+1)*local_m-1;

	for(i=my_first_row;i<=my_last_row;i++){
		y[i]=0.0;
		for (j = 0; j < n; j++) y[i]+=A[i][j]*x[j];
	}
	return NULL;
}

void* Tokenize(void* rank){
	long my_rank = (long) rank;
	int count ;
	int next= (my_rank+1)%thread_count;
	char *fg_rv;
	char my_line[MAX];
	char *my_string;

	sem_wait(&sems[my_rank]);
	fg_rv=fgets(my_line,MAX,stdin);
	sem_post(&sems[next]);
	while(fg_rv!=NULL){
		printf("Thread %1d > my line = %s \n",my_rank,my_line );

		count=0;
		my_string=strtok(my_line,"\t\n");
		while(my_string!=NULL){
			count++;
			printf("Thread %1d > string %d = %s \n", my_rank,count,my_string);
			my_string=strtok(NULL,"\t\n");
		}

		sem_wait(&sems[my_rank]);
		fg_rv=fgets(my_line,MAX,stdin);
		sem_post(&sems[next]);
	}

	return NULL;
}