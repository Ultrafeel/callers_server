
1. The essence of the task.

Implement the system - outgoing call generator (hereinafter referred to as IO).
IO is used for advertising campaigns. This is a hot call to the subscriber base with advertising offers.
The system consists of two parts - the server and the client.
The server is responsible for call logic and outgoing call implementation.
The client has an interface for managing the creation and launch of call campaigns. The real-time client displays information about the current status of the campaign that is currently running.

2. Requirements for implementation of the client part.

2.0 Use BOOST ASIO

2.1. When loading, the client opens the configuration file and reads the server connection parameters if the file is not available or has the wrong format - reports this with an alarm message and stops working, otherwise the transition to paragraph 2.2.

2.2. The client connects to the server and checks whether the server is available. You can choose how the server is available. If the server is unavailable, reports it with an alarm message and stops working, otherwise the transition to paragraph 2.3.

2.3. The client downloads a file containing campaign objects.
The campaign object contains the following data:
• Campaign name
• Campaign Priority
• Promotional text that you need to talk to the subscriber when successfully trying to dial
• List of subscribers to call (hereinafter referred to as the job). The list contains the following data:
- Subscriber name
- Subscriber's phone number
The maximum number of campaigns is not limited to functional requirements. You can choose the format of the description of the campaign object yourself. If the campaign description format is incorrect or an error occurred while trying to open the file - reports this with an alarm message and stops working, otherwise go to item 2.4.

2.4. The client establishes a connection to the server and sends to the server a data stream containing all the information about the campaigns described in the file. If there is a failure at the data transfer stage - reports this with an alarm message and stops working, otherwise the transition to item 2.5.

2.5. The client enters the display mode of the current status of running campaigns on the server. The information about the current running campaign is displayed. The fields "Campaign Name," "Campaign Priority" and information about the current job (subscriber name and number) should be displayed.

2.6. After completing the last campaign, the customer must inform that all tasks have been completed and complete.

3. Server Part Implementation Requirements.

3.1. When loading, the server opens the configuration file and reads the client connection parameters to it. If the file is opened or the format is incorrect - reports this with an alarm message and stops working, otherwise the transition to item 2.2.

3.2. The server starts listening on the port specified in the configuration file.

3.3. The server must be able to accept a data packet from the client containing an array of campaign objects. If the data packet could not be received to the end, it reports this with an alarm message and stops working, otherwise the transition to paragraph 3.4.

3.4. The server starts running campaigns. Campaigns are run in order of priority. If the priority is the same, a campaign containing fewer jobs is selected. If the number of jobs is also the same, the execution order is arbitrary.

3.5. Running a campaign involves calling all tasks in the order they are defined in the campaign object and "playing" the text of the advertising message to the off-hook subscriber. Set the playback duration so that the user can listen to the text to the end.

3.6. The server informs the client of the current running campaign (see item 2.5).

4. General requirements.

The application must be running in a linux environment. The application is console. It is not necessary to implement physical calls, it is enough to make an emulation that will be clearly displayed on the client. For simplicity, we believe that all calls within the campaign are successful. In the accompanying letter, indicate in which environment it was assembled (compiler, kernel version, used libraries).

