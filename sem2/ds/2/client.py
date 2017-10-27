import requests

# Send data
mes = raw_input("Enter filename: ") #receive user input
message = requests.get('http://localhost:8080/' + mes)	#using requests module to request data from server 
message.status_code	#status codes to check the validity of data
print message.text	#print the received text data