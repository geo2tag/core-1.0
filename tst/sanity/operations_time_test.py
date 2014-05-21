#!/usr/bin/env python

import urllib2
import sys
import json
import time
import random


def login():
    url = "http://" + sys.argv[1] + "/service/login"
    credentials = {}
    credentials['login'] = "Mark"
    credentials['password'] = "test"
    credentials = json.dumps(credentials)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, credentials, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Login failed."
        print resp[u'errno']
        sys.exit(-1)
    auth_token = resp['auth_token']
    f.close()
    return auth_token


def add_channel(name, auth_token):
    url = "http://" + sys.argv[1] + "/service/addChannel"
    channel = {}
    channel["auth_token"] = auth_token
    channel['name'] = name
    channel['description'] = 'my new super chanel'
    channel['url'] = 'http://osll.spb.ru'
    channel['activeRadius'] = 3000
    channel = json.dumps(channel)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, channel, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Add channel failed."
        print resp[u'errno']
        sys.exit(-1)


def subscribe_channel(channel, auth_token):
    url = "http://" + sys.argv[1] + "/service/subscribe"
    obj = {}
    obj["auth_token"] = auth_token
    obj["channel"] = channel
    obj = json.dumps(obj)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, obj, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())

def write_tag(channel, title, auth_token):
    url = "http://" + sys.argv[1] + "/service/writeTag"
    tag = {}
    tag["auth_token"] = auth_token
    tag['description'] = 'my new super tag'
    tag['channel'] = channel
    tag['latitude'] = 60.0
    tag['altitude'] = 30.0
    tag['longitude'] = 30.0
    tag['link'] = 'unknown'
    tag['time'] = '04 03 2011 15:33:47.630'
    tag['title'] = title
    tag = json.dumps(tag)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, tag, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Add tag failed."
        print resp[u'errno']
        sys.exit(-1)


def load_tags(channel, auth_token):
    url = "http://" + sys.argv[1] + "/service/filterChannel"
    obj = {}
    obj["auth_token"] = auth_token
    obj["channel"] = channel
    obj["amount"] = 100
    obj = json.dumps(obj)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, obj, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Load tag failed."
        print resp[u'errno']
        sys.exit(-1)
    else:
        return resp[u'channel'][u'items']
    
def doOperation(formula):
    url = "http://" + sys.argv[1] + "/service/channelsOperations"
    obj = {}
    obj["auth_token"] = auth_token
    obj["formula"] = formula
    obj = json.dumps(obj)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, obj, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Operation failed."
        print resp[u'errno']
        sys.exit(-1)
    else:
        return resp[u'tags']
        
def get_tags(start, end):
    tags = []
    for i in range(start, end+1):
        tag = 'tag' + str(i)
        tags.append(tag)
    return tags
    
def write_tags_to_channel(auth_token, channel, tags):
    for tag in tags:
        write_tag(channel, tag, auth_token)
  
if __name__ == '__main__':
    channel_1 = 'channel_' + str(random.randint(0, 10000))
    channel_2 = 'channel_' + str(random.randint(0, 10000))
    channel_3 = 'channel_' + str(random.randint(0, 10000))
    
    auth_token = login()
    
    add_channel(channel_1, auth_token)
    add_channel(channel_2, auth_token)
    add_channel(channel_3, auth_token)
    
    subscribe_channel(channel_1, auth_token)
    subscribe_channel(channel_2, auth_token)
    subscribe_channel(channel_3, auth_token)
    
    amount = 100
    file_union = open('union.csv', 'w')
    file_intersection = open('intersection.csv', 'w')
    file_complement = open('complement.csv', 'w')
    
    while amount <= 100000:
        time_union = 0
        time_intersection = 0
        time_complement = 0
        
        tags_1 = get_tags(1, amount)
        tags_2 = get_tags(amount/4, (amount * 5)/4)
        tags_3 = get_tags(amount/2, (amount * 3)/2)
        write_tags_to_channel(auth_token, channel_1, tags_1)
        write_tags_to_channel(auth_token, channel_2, tags_2)
        write_tags_to_channel(auth_token, channel_3, tags_3)
        
        for i in range(10):           
            
            start = time.time()
            union = doOperation(channel_1 + "|" + channel_1)
            end = time.time()
            time_union = time_union + (end - start)
            
            start = time.time()
            intersection = doOperation(channel_2 + "&" + channel_3)
            end = time.time()
            time_intersection = time_intersection + (end - start)
            
            start = time.time()
            complement = doOperation(channel_1 + "\\" + channel_3)
            end = time.time()
            time_complement = time_complement + (end - start)
        
        file_union.write(str(amount) + ', ' + str(time_union/100) + '\n')
        file_intersection.write(str(amount) + ', ' + str(time_intersection/100) + '\n')
        file_complement.write(str(amount) + ', ' + str(time_complement/100) + '\n')
        
        print amount, ' done'
        
        amount = amount * 10
    
    file_union.close()
    file_intersection.close()
    file_complement.close()
    print 'Success'
        