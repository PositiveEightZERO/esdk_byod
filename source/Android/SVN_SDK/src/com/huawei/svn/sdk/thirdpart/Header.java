/*
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.
 *  The ASF licenses this file to You under the Apache License, Version 2.0
 *  (the "License"); you may not use this file except in compliance with
 *  the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

package com.huawei.svn.sdk.thirdpart;

import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.logging.Logger;


/**
 * The general structure for request / response header. It is essentially
 * constructed by hashtable with key indexed in a vector for position lookup.
 * 
 * @author l00174413
 * @version 1.0
 * @see [相关类/方法]（可选）
 * @since [产品/模块版本] （必须）
 */
public class Header implements Cloneable
{
    
    /** The props. */
    private List<String> props;

    /** The key table. */
    private SortedMap<String, LinkedList<String>> keyTable;

    /** The status line. */
    private String statusLine;

    /**
     * A generic header structure. Used mostly for request / response header.
     * The key/value pair of the header may be inserted for later use. The key
     * is stored in an array for indexed slot access.
     */
    public Header()
    {
        super();
        this.props = new ArrayList<String>(20);
        this.keyTable = new TreeMap<String, LinkedList<String>>(
                String.CASE_INSENSITIVE_ORDER);
    }

    /**
     * The alternative constructor which sets the input map as its initial
     * keyTable.
     * 
     * @param map
     *            the initial keyTable as a map
     */
    public Header(Map<String, List<String>> map)
    {
        this(); // initialize fields
        String key = null;
        List<String> value = null;
        LinkedList<String> linkedList = null;
        for (Entry<String, List<String>> next : map.entrySet())
        {
            key = next.getKey();
            value = next.getValue();
            linkedList = new LinkedList<String>();
            for (String element : value)
            {
                linkedList.add(element);
                props.add(key);
                props.add(element);
            }
            keyTable.put(key, linkedList);
        }
    }

    /* (non-Javadoc)
     * @see java.lang.Object#clone()
     */
    @SuppressWarnings("unchecked")
    @Override
    public Object clone() throws CloneNotSupportedException
    {
        Header clone = (Header) super.clone();
        clone.props = (ArrayList<String>) ((ArrayList<String>) props).clone();
        clone.keyTable = new TreeMap<String, LinkedList<String>>(
                String.CASE_INSENSITIVE_ORDER);
        for (Map.Entry<String, LinkedList<String>> next : this.keyTable
                .entrySet())
        {
            LinkedList<String> v = (LinkedList<String>) next.getValue().clone();
            clone.keyTable.put(next.getKey(), v);
        }
        return clone;

    }

    /**
     * Add a field with the specified value.
     * 
     * @param key
     *            the key
     * @param value
     *            the value
     */
    public void add(String key, String value)
    {
//        if (key == null)
//        {
//            throw new NullPointerException();
//        }
        if (value == null)
        {
            /*
             * Given null values, the RI sends a malformed header line like
             * "Accept\r\n". For platform compatibility and HTTP compliance, we
             * print a warning and ignore null values.
             */
            Logger.getAnonymousLogger().warning(
                    "Ignoring HTTP header field " + key
                            + " because its value is null.");
            return;
        }
        LinkedList<String> list = keyTable.get(key);
        if (list == null)
        {
            list = new LinkedList<String>();
            keyTable.put(key, list);
        }
        list.add(value);
        props.add(key);
        props.add(value);
    }

    /**
     * 删除 all.
     * 
     * @param key
     *            the key
     */
    public void removeAll(String key)
    {
        keyTable.remove(key);

        for (int i = 0; i < props.size(); i += 2)
        {
            if (key.equals(props.get(i)))
            {
                props.remove(i); // key
                props.remove(i); // value
            }
        }
    }

    /**
     * 添加 all.
     * 
     * @param key
     *            the key
     * @param headers
     *            the headers
     */
    public void addAll(String key, List<String> headers)
    {
        for (String header : headers)
        {
            add(key, header);
        }
    }

    /**
     * 添加 if absent.
     * 
     * @param key
     *            the key
     * @param value
     *            the value
     */
    public void addIfAbsent(String key, String value)
    {
        if (get(key) == null)
        {
            add(key, value);
        }
    }

    /**
     * Set a field with the specified value. If the field is not found, it is
     * added. If the field is found, the existing value(s) are overwritten.
     * 
     * @param key
     *            the key
     * @param value
     *            the value
     */
    public void set(String key, String value)
    {
        removeAll(key);
        add(key, value);
    }

    /**
     * Provides an unmodifiable map with all String header names mapped to their
     * String values. The map keys are Strings and the values are unmodifiable
     * Lists of Strings.
     * 
     * @return an unmodifiable map of the headers
     * 
     * @since 1.4
     */
    public Map<String, List<String>> getFieldMap()
    {
        Map<String, List<String>> result = new TreeMap<String, List<String>>(
                String.CASE_INSENSITIVE_ORDER); // android-changed
        List<String> v = null;
        for (Map.Entry<String, LinkedList<String>> next : keyTable.entrySet())
        {
            v = next.getValue();
            result.put(next.getKey(), Collections.unmodifiableList(v));
        }
        return Collections.unmodifiableMap(result);
    }

    /**
     * Returns the element at <code>pos</code>, null if no such element exist.
     * 
     * @param pos
     *            int the position to look for
     * @return java.lang.String the value of the key
     */
    public String get(int pos)
    {
        if (pos >= 0 && pos < props.size() / 2)
        {
            return props.get(pos * 2 + 1);
        }
        return null;
    }

    /**
     * Returns the key of this header at <code>pos</code>, null if there are
     * fewer keys in the header.
     * 
     * @param pos
     *            the position to look for
     * @return the key the desired position
     */
    public String getKey(int pos)
    {
        if (pos >= 0 && pos < props.size() / 2)
        {
            return props.get(pos * 2);
        }
        return null;
    }

    /**
     * Returns the value corresponding to the specified key, or null.
     * 
     * @param key
     *            the key
     * @return the string
     */
    public String get(String key)
    {
        LinkedList<String> result = keyTable.get(key);
        if (result == null)
        {
            return null;
        }
        return result.getLast();
    }

    /**
     * Returns the number of keys stored in this header.
     * 
     * @return the number of keys.
     */
    public int length()
    {
        return props.size() / 2;
    }

    /**
     * Sets the status line in the header request example: GET / HTTP/1.1
     * response example: HTTP/1.1 200 OK
     * 
     * @param statusLine
     *            the new status line
     */
    public void setStatusLine(String statusLine)
    {
        this.statusLine = statusLine;
        /*
         * we add the status line to the list of headers so that it is
         * accessible from java.net.HttpURLConnection.getResponseCode() which
         * calls
         * org.apache.harmony.luni.internal.net.www.protocol.http.HttpURLConnection
         * .getHeaderField(0) to get it
         */
        props.add(0, null);
        props.add(1, statusLine);
    }

    /**
     * Gets the status line in the header request example: GET / HTTP/1.1
     * response example: HTTP/1.1 200 OK
     * 
     * @return the status line
     */
    public String getStatusLine()
    {
        return statusLine;
    }
}