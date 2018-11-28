int get_output_reaction(int success, int tries, int input)
{
		if(input == 1)
		{
			tries++;
			return 1;
		}
		else if(input == 0 && (tries < 3 || success) )
		{
			success = 1;
			return 0;
		}
		else 
		{
			tries = 0;
			return 1;
		}
}

int main()
{
	return get_output_reaction(0,1,0);
	//return pap_state_imp();
}
