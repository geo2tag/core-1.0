#!/usr/bin/env python

import urllib2
import sys
import json
import time


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
    

def union(channel_1, channel_2, auth_token):
    url = "http://" + sys.argv[1] + "/service/channelsUnion"
    obj = {}
    obj["auth_token"] = auth_token
    obj["channel_1"] = channel_1
    obj["channel_2"] = channel_2
    obj = json.dumps(obj)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, obj, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Union failed."
        print resp[u'errno']
        sys.exit(-1)
    else:
        return resp[u'tags']
        

def intersection(channel_1, channel_2, auth_token):
    url = "http://" + sys.argv[1] + "/service/channelsIntersection"
    obj = {}
    obj["auth_token"] = auth_token
    obj["channel_1"] = channel_1
    obj["channel_2"] = channel_2
    obj = json.dumps(obj)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, obj, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Union failed."
        print resp[u'errno']
        sys.exit(-1)
    else:
        return resp[u'tags']
        

def complement(channel_1, channel_2, auth_token):
    url = "http://" + sys.argv[1] + "/service/channelsComplement"
    obj = {}
    obj["auth_token"] = auth_token
    obj["channel_1"] = channel_1
    obj["channel_2"] = channel_2
    obj = json.dumps(obj)
    header = {'Content-type': 'application/json'}
    req = urllib2.Request(url, obj, header)
    f = urllib2.urlopen(req)
    resp = json.loads(f.read())
    if resp[u'errno'] != 0:
        print "Union failed."
        print resp[u'errno']
        sys.exit(-1)
    else:
        return resp[u'tags']
        
        
def compare(l1, l2):
    if len(l1) != len(l2):
        return False
    else:
       for k, j in zip(l1.sort(), l2.sort()):
           if k != j:
               return False
               
    return True

    
    
if __name__ == '__main__':
    channel_1 = 'channel_' + str(time.time())
    channel_2 = 'channel_' + str(time.time()+1)
    tags_union = [{u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag1', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0},
                  {u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag2', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0},
                  {u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag3', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0},
                  {u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag4', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0}]
                  
    tags_intersection = [{u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag1', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0},
                         {u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag2', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0}]
                         
    tags_complement = [{u'description': u'my new super tag', u'pubDate': u'04 03 2011 15:33:47.630', u'title': u'tag3', u'altitude': 30.0, u'longitude': 30.0, u'link': u'unknown', u'user': u'Mark', u'latitude': 60.0}]
    
    
    auth_token = login()
    add_channel(channel_1, auth_token)
    add_channel(channel_2, auth_token)
    subscribe_channel(channel_1, auth_token)
    subscribe_channel(channel_2, auth_token)
    write_tag(channel_1, 'tag1', auth_token)
    write_tag(channel_1, 'tag2', auth_token)
    write_tag(channel_1, 'tag3', auth_token)
    write_tag(channel_2, 'tag1', auth_token)
    write_tag(channel_2, 'tag2', auth_token)
    write_tag(channel_2, 'tag4', auth_token)
    load_tags(channel_1, auth_token)
    load_tags(channel_2, auth_token)
    u = union(channel_1, channel_2, auth_token)
    i = intersection(channel_1, channel_2, auth_token)
    c = complement(channel_1, channel_2, auth_token)
    
    if not compare(u, tags_union):
        print "Union failed."
        sys.exit(-1)
        
    if not compare(i, tags_intersection):
        print "Intersection failed."
        sys.exit(-1)
    
    if not compare(c, tags_complement):
        print "Complement failed."
        sys.exit(-1)    
    
        