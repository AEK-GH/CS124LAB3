#include "runtime.hpp"


runtime::runtime() :
	mainloop_state(false),
	cipher_present_state(false),
	data_present_state(false),
	file_save_state(false),
	encrypted_state(false)
{}

std::string runtime::file_input()
{
	std::cout << "ENTER: ";
	std::string file_name;
	bool verified = false;
	do
	{
		file_name = string_input();
		if(parser::does_this_file_exist(file_name))
		{
			verified = true;
		}
		else
		{
			std::cout <<
			std::endl << "File nonexistent, try again: ";
		}
	}while(!verified);
	return file_name;
}

//this function takes a string input
//verifying that it is not empty else
//prompting the user to enter something again
std::string runtime::string_input()
{
	std::string input;
	bool verified = false;
	do
	{
		std::getline(std::cin, input, '\n');
		if(input.length() == 0)
		{
			std::cout <<
		 	std::endl << "blank, try again: ";
		}
		else
		{
			verified = true;
		}
	}while(!verified);
	return input;
}

//COMPLETE
//this function uses string_input to return a bool based
//on user input
bool runtime::boolean_question()
{
	bool verified = false; //determines if question
			       //loops based on invalid answer
	bool answer; //value of the answer input by the user

	std::string input; //single variable needed to store
			   //input even over repeated iterations
			   //of prompt
	std::cout << "[y/n]: "; //prompt for user for valid characters,

	do{
		input = string_input(); //getting string from
					//validated input function
		if(input == "y") //checking for yes and setting bools
		{
			verified = true;
			answer = true;
		}
		else if(input == "n") //checking for no and
				      //setting bools
		{
			verified = true;
			answer = false;
		}
		else //informing and reprompting user
		     //in case of invalid input
		{
			std::cout << std::endl << "invalid, try again: ";
			verified = false;
		}
	}while(!verified); //prompt loop until a desired answer
			   //has been given

	return answer;
}



std::string runtime::file_question(std::string type)
{
	std::cout <<
	std::endl << "Please provide a '" << type << "' file with the .txt" <<
	std::endl << "extension, and ensure its the appropriate format." <<
	std::endl;
	return file_input();
}

bool runtime::file_reprompt(std::string type)
{
	std::cout <<
	std::endl << "Would you like to select a different[y] '" << type << "'" <<
	std::endl << "file or continue using the current[n] one?" <<
	std::endl;
	return boolean_question();
}


bool runtime::proceed_question()
{

	std::cout <<
	std::endl << "Greeting user, welcome to the Enigma Machine." <<
	std::endl << "This program allows you to provide a cipher" <<
	std::endl << "and provide plain-text files for encryption" <<
	std::endl << "or an encrypted file for decryption." <<
	std::endl <<
	std::endl << "Would you like to proceed[y] or exit[n]." <<
	std::endl;
	return boolean_question();
}

bool runtime::restart_question()
{
	std::cout <<
	std::endl << "You have reached the end of the program." <<
	std::endl <<
	std::endl << "Would you like to restart[y] or exit[n]." <<
	std::endl;
	return boolean_question();
}

void runtime::end_message()
{
	std::cout <<
	std::endl << "Thank you for using the Enigma Machine!" <<
	std::endl;
}

bool runtime::encrypted_question()
{
	std::cout <<
	std::endl << "You have provided an encrypted file." <<
	std::endl <<
	std::endl << "Would you like to save the decrypted version" <<
	std::endl << "of the text to the file?" <<
	std::endl;
	return boolean_question();
}

bool runtime::plaintext_question()
{
	std::cout <<
	std::endl << "You have provided an unencrypted file." <<
	std::endl <<
	std::endl << "Would you like to save the encrypted version" <<
	std::endl << "of the text to the file?" <<
	std::endl;
	return boolean_question();
}

void runtime::justified_print(std::string input, int width)
{
	std::cout << std::endl;
	for(int i = 0; i < input.size(); i++)
	{
		std::cout << input[i];
		if((i + 1) % width == 0)
		{
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}

void runtime::start()
{
	mainloop_state = proceed_question();
	while(mainloop_state)
	{

		if(!cipher_present_state)
		{

			cipher_file_name = file_question("cipher");
			cipher_present_state = true;
		}
		else if(file_reprompt("cipher"))
		{
			cipher_file_name = file_question("cipher");
		}

		parser cipher_file(cipher_file_name);

		std::vector<std::pair<char, std::string> > ciphers
			= cipher_file.parse_cipher();

		hashmap cipher_map(ciphers);
		//cipher_map.print();
		//Tree cipher_tree(ciphers); //allocation of existing cipher pairs

		Tree cipher_tree(ciphers);

		if(!data_present_state)
		{
			data_file_name = file_question("data");

			data_present_state = true;
		}
		else if(file_reprompt("data"))
		{
			data_file_name = file_question("data");
		}
		parser data_file(data_file_name);

		encrypted_state = parser::is_this_file_encrypted(data_file_name);

		if(encrypted_state)
		{
			tokens = data_file.parse_encryption();

			file_save_state = encrypted_question();
		}
		else
		{
			tokens = data_file.parse_plaintext();

			file_save_state = plaintext_question();
		}
		if(file_save_state)
		{
			std::string text;
			if(encrypted_state) { text = cryptor::decrypt(tokens, cipher_map); }
			else { text = cryptor::encrypt(tokens, cipher_tree); }

			// output results to cout and file
			data_file.append_to_file(text);
			justified_print(text, 45);	

			file_save_state = false; //reset for subsequent loops
		}

		mainloop_state = restart_question();
	}
	end_message();
}
