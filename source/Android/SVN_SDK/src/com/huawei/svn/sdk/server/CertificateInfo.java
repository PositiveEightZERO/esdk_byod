package com.huawei.svn.sdk.server;

public class CertificateInfo 
{
	private byte[] content;
	private String password;
	
	public CertificateInfo(byte[] content, String password) {
		super();
		this.content = content;
		this.password = password;
	}
	public byte[] getContent() {
		return content;
	}
	public void setContent(byte[] content) {
		this.content = content;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}

}
