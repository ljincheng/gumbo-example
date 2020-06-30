create table site_info_discover(ID INTEGER PRIMARY KEY AUTOINCREMENT,LINK NVARCHAR(200),TITLE NVARCHAR(100),BASEURL NVARCHAR(100),SITENAME NVARCHAR(100));

create table site_info_url(
ID INTEGER PRIMARY KEY AUTOINCREMENT
,LINK NVARCHAR(200)
,BASEURL NVARCHAR(200)
,SITENAME NVARCHAR(100)
,STATUS INTEGER
);

insert into site_info_url(LINK,BASEURL,SITENAME,STATUS) values
('http://www.stay86.com/cn/info-1.html','http://stay86.com/cn/information','STAY86',1),('https://en.whu.edu.cn/list.jsp?urltype=tree.TreeTempUrl&wbtreeid=1050','https://en.whu.edu.cn/info','清华大学',1);


