#!/usr/bin/python2.7
# fzn 2015

import subprocess, random, re				#for rnd
import pycurl, cStringIO, base64, urllib3, urllib	#for all

##################################
#             Options            #
##################################

host = "192.168.0.254"		# router host / ip
login = "root"
password = "hackmeplease"
maxTries = 3			# attempts before we abort
debug = True			# log requests on stdout
useProxy = True			# proxify through 192.168.1.1:8080
beVerbose = False		# run curl verbosely

##################################
#             Actions            #
##################################

def set_psk(id):	#Necessary:IntervalWpa
	d={"pskSecret":id,"pskSecOpt":"3","pskCipher":"1","interval":"86400", "sectype":"3","intervalWpa":"86400"}
	sendForm("WlanSecurity", d)

def set_ssid(id):
	sendForm("WlanNetwork", {"ssid1":id})

def set_opmode(id):
	opmodes = ["ap", "router", "repeater", "bridge", "client"]
	sendForm("WlanApMode", {"opmode":opmodes.index(id)+1})

#porttype:1 static ip, 2 smart ip
def set_host(h, nm="255.255.255.0", p="static"):
	porttypes= ["static", "smart"]
	d={"porttype":porttypes.index(p)+1,"lanip":h,"lanmask":"2","inputMask":nm}
	sendForm("NetworkCfg", d)

def reboot():
	sendForm("SysReboot", {"Reboot":"Reboot"})

def change_pwd(ou, op, nu, np):
	d={"oldname":ou,"oldpassword":op,"newname":nu,"newpassword":np,"newpassword2":np,"Save":"Save"}
	sendForm("ChangeLoginPwd", d)

def rnd():
	phrase = subprocess.check_output(["fortune", "-o"])
	words = re.split(ur"[\s,]+", phrase, flags = re.UNICODE)
	s = words[random.randint(0, len(words) - 1)]
	k=''.join(c for c in s if c.islower() or c.isupper())
	return rnd() if len(k) < 8 else  k

##################################
#      Main & net routines       #
##################################

#GET url?dat 
def do_get(url, dat):
        headers = { 'Authorization' : 'Basic %s' % base64.b64encode(login+":"+password) }
        response = cStringIO.StringIO()
        c = pycurl.Curl()
        if useProxy is True:
                c.setopt(pycurl.PROXY, '127.0.0.1')
                c.setopt(pycurl.PROXYPORT, 8080)
        if beVerbose is True: c.setopt(pycurl.VERBOSE, 1)
        c.setopt(pycurl.HTTPHEADER, ["%s: %s" % t for t in headers.items()])
        c.setopt(pycurl.URL, url+"?"+dat)
        c.setopt(pycurl.REFERER, url)
        c.setopt(pycurl.WRITEFUNCTION, response.write)
        c.perform()
        r = int(c.getinfo(pycurl.HTTP_CODE))
	if r == 401 or r == 502: raise ValueError(str(r))

#prepare url and flattens parameters
def sendForm(ur, d):
        if debug is True: print "-[c]->"+str(d)
        d.update({"Save":"Save"})
        serd = urllib.urlencode(d)
        url = "http://"+host+"/userRpm/"+ur+"Rpm.htm" #+"?"+serd
        do_get(url,serd)

def attemptReset():
	global host, login, password
	origPass = password
	origLogi = login
	origHost = host
	login = "admin"
	password = "admin"
	host = "192.168.0.254"
	print "-[!]->Will attempt to recover from reset"
	change_pwd("admin", "admin", origLogi, origPass)	
	set_host(origHost)
	host, password, login = origHost, origPass, origLogi
#	password = origPass
#	login = origLogi

def actions():
	set_ssid(rnd())
	set_psk(rnd())
	set_opmode("bridge")
	set_host("192.168.0.22")
	reboot()

def main():
	tries = 0
        while True:
		try:
			actions()
                except ValueError as e:
			if int(e.args[0]) == 502:
				print "-[!]->No route to host"
				return
			try:
				tries += 1
				attemptReset()
				if tries > maxTries:
					print "-[!]->Max Tries (" + str(maxTries) + ") reached. Check your credentials."
					print "-[!]->...Is it properly plugged in? Abort."
					pass
				else:
					continue
			except ValueError:
				print "-[!]->Better reset it"
		break

main()
