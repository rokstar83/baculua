require("baculua")

mon = baculua.get_monitor("homer-mon", "homer", "iris", "nottheactualpassword")
msg = baculua.director_status(mon)

if(msg == "SUCCESS") then
	 print("Passed")
else
	 print("Failed")
end
