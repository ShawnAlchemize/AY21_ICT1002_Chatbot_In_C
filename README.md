# AY21_ICT1002_Chatbot_In_C
AY21_ICT1002_Chatbot_In_C

### Objective
Chatbots – computer programs that can respond to queries typed in natural language – have recently
become a popular addition to web sites and other interactive applications.
The aim of this project is to implement a simple chatbot that can respond to simple queries made up of
a single question word (called the intent) and an object (called the entity). The chatbot will be able to
answer questions such as Where is the printer? or What is C? by identifying the intent (where or what)
and entity (printer or C) then looking up the answer corresponding to this pair in a simple database.
The chatbot will also be able to learn new answers by asking questions of the user. If a user asks a
question for which the database does not contain an answer, the user will be given the option to supply
an answer that can be used to respond to the same question in future. (This simulates having the chatbot
refer the unknown question to a third party who does know the answer.)


## Error Handling
- fget buffer overflow
- user command action checking ("are you sure you want to...")
- y/n error handling (only accepts y or n)
- allows for update of entities
- check for save file overwriting
- check if user wants to save before exiting 


## Time and deliverables
### By 11:30pm, 2 Dec 2021, Thursday
Submit a zip file to your group’s xSiTe Dropbox, including:
- All C source code
- Report
- Project presentation and demonstration video (maximum 10 minutes)
Instructions on the “Project presentation and demonstration video” will be shared at a later time.
The zip file should be named in format of: ICT1002_C_GP_Team 49.zip.

### By 11:30pm, 3 Dec 2021, Friday
- Peer Evaluation 
"Complete your peer evaluation on TEAMMATES. Otherwise, the contribution credits will be evenly distributed among the team members."


### Demonstration:
https://youtu.be/7Rm4nXf-bGA

#### To build the project in Visual Studio Code, in the integrated terminal, enter:
gcc -o main *.c