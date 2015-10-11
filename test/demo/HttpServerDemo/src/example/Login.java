/*
 * $Id: Login.java 471756 2006-11-06 15:01:43Z husted $
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package example;

import java.text.SimpleDateFormat;
import java.util.Date;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.ServletActionContext;


//import net.sf.json.JSONObject;  

//@SuppressWarnings("serial")
public class Login extends ExampleSupport {
	
	private String username;

    public String execute() throws Exception {

        if (isInvalid(getUsername())) 
        {
//        	JSONObject jsonObject=new JSONObject();  
//
//             jsonObject.accumulate("Result", "User name Error!");  
//
//        	//这里在request对象中放了一个data，所以struts的result配置中不能有type="redirect"  
//
//        	ServletActionContext.getRequest().setAttribute("Error", jsonObject.toString());  

        	return INPUT;
        }

        if (isInvalid(getPassword()))  
        {
//        	JSONObject jsonObject=new JSONObject();  
//
//            jsonObject.accumulate("Result", "Password Error!");  
//
//	       	//这里在request对象中放了一个data，所以struts的result配置中不能有type="redirect"  
//	
//	       	ServletActionContext.getRequest().setAttribute("Error", jsonObject.toString());  
	
	       	return INPUT;
       }
        
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");//设置日期格式
        
       
        
        Cookie cookie = new Cookie("testcookie", getUsername() + df.format(new Date()));
        
        
        ServletActionContext.getResponse().addCookie(cookie);


        HttpServletRequest request = ServletActionContext.getRequest ();
        
        request.getSession().setAttribute("username", username);
        
        request.getSession().setAttribute("password", password);
      
        return SUCCESS;
    }

    private boolean isInvalid(String value) {
        return (value == null || value.length() == 0);
    }

    //private String username;

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    private String password;

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

}