#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/time.h>
#include<unistd.h>


#define BUFMAX 40
#define PORT_NO (u_short)20000
#define Err(x) {fprintf(stderr,"-"); perror(x); exit(0);}

static int n,number,sofd,nsofd,retval;
static struct hostent *shost;
static struct sockaddr_in own,svaddr;
static char buf[BUFMAX],shostn[BUFMAX];
static fd_set mask;
static struct timeval tm;

int map[7][6];


int servpro();
int cliepro();
int vertical();
int horizontal();
int diagonal();

unsigned long MyColor(Display *display,char *color){
  Colormap cmap;
  XColor c0,c1;
  cmap=DefaultColormap(display,0);
  XAllocNamedColor(display,cmap,color,&c1,&c0);
  return (c1.pixel);
}

int main(){

  for(int i=0;i<7;i++){
    for(int j=0;j<6;j++){
      map[i][j]=0;
    }
  }
  
  while(1){
    printf("server:1 client:2\n");
    scanf("%d",&number);
    printf("\n");
    if(number==1||number==2) break;
  }

  if(number==1) servpro();
  if(number==2) cliepro();
}

int servpro(){
  Display *d;
  Window w;
  XEvent event;
  GC gc;
  XGCValues gv;
  Colormap cmap;
  unsigned long MyColor();
  unsigned long darkorange,darkgoldenrod,chocolate,coral,peru,sandybrown,sienna,black,white;
  d=XOpenDisplay(NULL);
  darkorange=MyColor(d,"darkorange");
  darkgoldenrod=MyColor(d,"darkgoldenrod");
  chocolate=MyColor(d,"chocolate");
  coral=MyColor(d,"coral");
  peru=MyColor(d,"peru");
  sandybrown=MyColor(d,"sandybrown");
  sienna=MyColor(d,"sienna");
  black=MyColor(d,"black");
  white=MyColor(d,"white");
  tm.tv_sec=0;
  tm.tv_usec=1;

  if(gethostname(shostn,sizeof(shostn))<0)
    Err("gethostname");

  printf("hostname is %s",shostn);
  printf("\n");

  shost=gethostbyname(shostn);
  if(shost==NULL) Err("gethostbyname");

  bzero((char *)&own,sizeof(own));
  own.sin_family=AF_INET;
  own.sin_port=htons(PORT_NO);
  bcopy((char *)shost->h_addr,(char *)&own.sin_addr,shost->h_length);

  sofd=socket(AF_INET,SOCK_STREAM,0);
  if(sofd<0) Err("socket");
  if(bind(sofd,(struct sockaddr*)&own,sizeof(own))<0) Err("bird");

  listen(sofd,1);

  nsofd=accept(sofd,NULL,NULL);
  if(nsofd<0) Err("accept");
  close(sofd);

  write(1,"START\n",6);

  w=XCreateSimpleWindow(d,RootWindow(d,0),20,20,600,600,2,
			BlackPixel(d,DefaultScreen(d)),
			WhitePixel(d,DefaultScreen(d)));
  XSelectInput(d,w,ExposureMask|KeyPressMask|ButtonPressMask);
  XMapWindow(d,w);
  XStoreName(d,w,"Server : black");
  gv.line_width=1;
  gc=XCreateGC(d,w,GCLineWidth,&gv);
  XArc arca[500],arcb[500];
  int a=0,b=0,c=0;
  int flag=0;
  int key=0;
  char count[100];

  while(1){
    FD_ZERO(&mask);
    FD_SET(nsofd,&mask);
    FD_SET(0,&mask);
    retval=select(nsofd+1,&mask,NULL,NULL,&tm);

    if(XPending(d)!=0){
      XNextEvent(d,&event);
      switch(event.type){
      case Expose:
	if(key%7==0){
	  XSetForeground(d,gc,darkorange);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==1){
	  XSetForeground(d,gc,darkgoldenrod);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==2){
	  XSetForeground(d,gc,chocolate);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==3){
	  XSetForeground(d,gc,coral);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==4){
	  XSetForeground(d,gc,peru);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==5){
	  XSetForeground(d,gc,sandybrown);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else{
	  XSetForeground(d,gc,sienna);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}
	XSetForeground(d,gc,black);
	for(int i=150;i<=500;i+=50)
	  XDrawLine(d,w,gc,i,200,i,500);
	for(int j=200;j<=500;j+=50)
	  XDrawLine(d,w,gc,150,j,500,j);
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,450,100,50,50);
	XFillArcs(d,w,gc,arca,b);
	XSetForeground(d,gc,white);
	XFillArcs(d,w,gc,arcb,c);
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,150,510,350,20);
	XFillRectangle(d,w,gc,150,170,350,20);
	XSetForeground(d,gc,white);
	XDrawString(d,w,gc,155,520,"You have just started the game",
		    strlen("You have just started the game"));
	XDrawString(d,w,gc,155,528,"(or your windowsettings have just been changed).",
		    strlen("(or your windowsettings have just been changed)."));
	sprintf(count,"Black stones : %d , White stones : %d",b,c);
	XDrawString(d,w,gc,225,180,count,strlen(count));
	break;

	
      case ButtonPress:
	if(event.xbutton.x<500 && event.xbutton.x>150 && event.xbutton.y<500 && event.xbutton.y>200){
	  if(((event.xbutton.y/50-4)==5||map[event.xbutton.x/50-3][event.xbutton.y/50-3]!=0)&&b==c){
	    for(int p=0;p<b;p++){
	      if(arca[p].x==50*(event.xbutton.x/50) && arca[p].y==50*(event.xbutton.y/50)){
		flag=1;
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,150,510,350,20);
		XSetForeground(d,gc,white);
		XDrawString(d,w,gc,155,520,"That's where you've already placed the stones!",
			    strlen("That's where you've already placed the stones!"));
		break;
	      }
	    }
	    for(int p=0;p<c;p++){
	      if(arcb[p].x==50*(event.xbutton.x/50) && arcb[p].y==50*(event.xbutton.y/50)){
		flag=1;
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,150,510,350,20);
		XSetForeground(d,gc,white);
		XDrawString(d,w,gc,155,520,"That's where client has already placed the stones!",
			    strlen("That's where client has already placed the stones!"));
		break;
	      }
	    }
	    if(flag){
	      flag=0;
	      break;
	    }
	    arca[b].x=50*(event.xbutton.x/50);
	    arca[b].y=50*(event.xbutton.y/50);
	    arca[b].width=50;
	    arca[b].height=50;
	    arca[b].angle1=0;
	    arca[b].angle2=360*64;
	    XSetForeground(d,gc,black);
	    map[event.xbutton.x/50-3][event.xbutton.y/50-4]=1;
	    XFillArc(d,w,gc,arca[b].x,arca[b].y,50,50,0,360*64);
	    b++;
	    
	    
	    sprintf(buf,"PLACE-%d%d",event.xbutton.x/50-3,event.xbutton.y/50-4);
	    n=sizeof(buf);
	    write(nsofd,buf,n);
	    printf("you:%s\n",buf);
	    
	    XSetForeground(d,gc,black);
	    XFillRectangle(d,w,gc,150,510,350,20);
	    XFillRectangle(d,w,gc,150,170,350,20);
	    XSetForeground(d,gc,white);
	    XDrawString(d,w,gc,155,520,"You have placed a black stone.",
			strlen("You have placed a black stone."));
	    sprintf(count,"Black stones : %d , White stones : %d",b,c);
	    XDrawString(d,w,gc,225,180,count,strlen(count));
	  }
	}else if(event.xbutton.x<=500 && event.xbutton.x>=450 && event.xbutton.y<=150 && event.xbutton.y>=100){
	  key++;
	  if(key%7==0){
	    XSetForeground(d,gc,darkorange);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==1){
	    XSetForeground(d,gc,darkgoldenrod);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==2){
	    XSetForeground(d,gc,chocolate);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==3){
	    XSetForeground(d,gc,coral);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==4){
	    XSetForeground(d,gc,peru);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==5){
	    XSetForeground(d,gc,sandybrown);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else{
	    XSetForeground(d,gc,sienna);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }
	  XSetForeground(d,gc,black);
	  for(int i=150;i<=500;i+=50)
	    XDrawLine(d,w,gc,i,200,i,500);
	  for(int j=200;j<=500;j+=50)
	    XDrawLine(d,w,gc,150,j,500,j);
	  XFillArcs(d,w,gc,arca,b);
	  XSetForeground(d,gc,white);
	  XFillArcs(d,w,gc,arcb,c);
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"You've changed the color of board.",
		      strlen("You've changed the color of board."));
	  break;
	}else{
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"You're clicking out of range!",
		      strlen("You're clicking out of range!"));
	}
	break;
      }
      bzero(buf,BUFMAX);
    }

    if(FD_ISSET(nsofd,&mask)){
      n=read(nsofd,buf,BUFMAX);
      if(buf[0]=='P'&&buf[1]=='L'&&buf[2]=='A'&&buf[3]=='C'&&buf[4]=='E'&&buf[5]=='-'&&buf[6]>='0'&&buf[6]<='6'&&buf[7]>='0'&&buf[7]<='5'){
	char tmp1[10],tmp2[10];
	sprintf(tmp1,"%c",buf[6]);
	sprintf(tmp2,"%c",buf[7]);
	for(int p=0;p<b;p++){
	  if(arca[p].x==150+50*atoi(tmp1) && arca[p].y==200+50*atoi(tmp2)){
	    flag=1;
	    break;
	  }
	}
	for(int p=0;p<c;p++){
	  if(arcb[p].x==150+50*atoi(tmp1) && arcb[p].y==200+50*atoi(tmp2)){
	    flag=1;
	    break;
	  }
	}
	if(flag==1||b==c||(map[atoi(tmp1)][atoi(tmp2)+1]==0&&atoi(tmp2)!=5)){
	  flag=0;
	  printf("\t\tclient:%s\n",buf);
	  sprintf(buf,"ERROR");
	  n=sizeof(buf);
	  write(nsofd,buf,n);
	  close(nsofd);
	  printf("you:%s\n",buf);
	  printf("DISCONNECT\n");
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XFillRectangle(d,w,gc,150,170,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
		      strlen("ERROR. Press any key to exit."));
	  while(1){
	    if(XPending(d)!=0){
	      XNextEvent(d,&event);
	      switch(event.type){
		case KeyPress:
		  exit(0);
	      case Expose:
		if(key%7==0){
		  XSetForeground(d,gc,darkorange);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==1){
		  XSetForeground(d,gc,darkgoldenrod);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==2){
		  XSetForeground(d,gc,chocolate);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==3){
		  XSetForeground(d,gc,coral);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==4){
		  XSetForeground(d,gc,peru);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==5){
		  XSetForeground(d,gc,sandybrown);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else{
		  XSetForeground(d,gc,sienna);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}
		XSetForeground(d,gc,black);
		for(int i=150;i<=500;i+=50)
		  XDrawLine(d,w,gc,i,200,i,500);
		for(int j=200;j<=500;j+=50)
		  XDrawLine(d,w,gc,150,j,500,j);
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,450,100,50,50);
		XFillArcs(d,w,gc,arca,b);
		XSetForeground(d,gc,white);
		XFillArcs(d,w,gc,arcb,c);
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,150,510,350,20);
		XFillRectangle(d,w,gc,150,170,350,20);
		XSetForeground(d,gc,white);
		XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
			    strlen("ERROR. Press any key to exit."));
		break;
	      }
	    }
	  }
	}
	else{
	  arcb[c].x=150+50*atoi(tmp1);
	  arcb[c].y=200+50*atoi(tmp2);
	  arcb[c].width=50;
	  arcb[c].height=50;
	  arcb[c].angle1=0;
	  arcb[c].angle2=360*64;
	  XSetForeground(d,gc,white);
	  map[atoi(tmp1)][atoi(tmp2)]=2;
	  XFillArc(d,w,gc,arcb[c].x,arcb[c].y,50,50,0,360*64);
	  c++;
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XFillRectangle(d,w,gc,150,170,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"Now your turn.",
		      strlen("Now your turn."));
	  sprintf(count,"Black stones : %d , White stones : %d",b,c);
	  XDrawString(d,w,gc,225,180,count,strlen(count));
	  printf("\t\tclient:%s\n",buf);
	  int x=vertical(),y=horizontal(),z=diagonal();
	  if(x==2||y==2||z==2){
	    sprintf(buf,"YOU-WIN");
	    n=sizeof(buf);
	    write(nsofd,buf,n);
	    printf("YOU-LOSE\n");
	    close(nsofd);
	    printf("DISCONNECT\n");
	    XSetForeground(d,gc,black);
	    XFillRectangle(d,w,gc,150,510,350,20);
	    XFillRectangle(d,w,gc,150,170,350,20);
	    XSetForeground(d,gc,white);
	    XDrawString(d,w,gc,155,520,"You lose. Press any key to exit.",
			strlen("You lose. Press any key to exit."));
	    while(1){
	      if(XPending(d)!=0){
		XNextEvent(d,&event);
		switch(event.type){
		case KeyPress:
		  exit(0);
		case Expose:
		  if(key%7==0){
		    XSetForeground(d,gc,darkorange);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==1){
		    XSetForeground(d,gc,darkgoldenrod);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==2){
		    XSetForeground(d,gc,chocolate);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==3){
		    XSetForeground(d,gc,coral);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==4){
		    XSetForeground(d,gc,peru);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==5){
		    XSetForeground(d,gc,sandybrown);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else{
		    XSetForeground(d,gc,sienna);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }
		  XSetForeground(d,gc,black);
		  for(int i=150;i<=500;i+=50)
		    XDrawLine(d,w,gc,i,200,i,500);
		  for(int j=200;j<=500;j+=50)
		    XDrawLine(d,w,gc,150,j,500,j);
		  XSetForeground(d,gc,black);
		  XFillRectangle(d,w,gc,450,100,50,50);
		  XFillArcs(d,w,gc,arca,b);
		  XSetForeground(d,gc,white);
		  XFillArcs(d,w,gc,arcb,c);
		  XSetForeground(d,gc,black);
		  XFillRectangle(d,w,gc,150,510,350,20);
		  XFillRectangle(d,w,gc,150,170,350,20);
		  XSetForeground(d,gc,white);
		  XDrawString(d,w,gc,155,520,"You lose. Press any key to exit.",
			      strlen("You lose. Press any key to exit."));
		  break;
		}
	      }
	    }
	  }else if(b+c==42){
	    sprintf(buf,"ERROR");
	    n=sizeof(buf);
	    write(nsofd,buf,n);
	    printf("you:%s\n",buf);
	    XSetForeground(d,gc,black);
	    XFillRectangle(d,w,gc,150,510,350,20);
	    XFillRectangle(d,w,gc,150,170,350,20);
	    XSetForeground(d,gc,white);
	    XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
			strlen("ERROR. Press any key to exit."));
	    while(1){
	      if(XPending(d)!=0){
		XNextEvent(d,&event);
		switch(event.type){
		case KeyPress:
		  exit(0);
		case Expose:
		  if(key%7==0){
		    XSetForeground(d,gc,darkorange);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==1){
		    XSetForeground(d,gc,darkgoldenrod);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==2){
		    XSetForeground(d,gc,chocolate);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==3){
		    XSetForeground(d,gc,coral);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==4){
		    XSetForeground(d,gc,peru);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==5){
		    XSetForeground(d,gc,sandybrown);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else{
		    XSetForeground(d,gc,sienna);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }
		  XSetForeground(d,gc,black);
		  for(int i=150;i<=500;i+=50)
		    XDrawLine(d,w,gc,i,200,i,500);
		  for(int j=200;j<=500;j+=50)
		    XDrawLine(d,w,gc,150,j,500,j);
		  XSetForeground(d,gc,black);
		  XFillRectangle(d,w,gc,450,100,50,50);
		  XFillArcs(d,w,gc,arca,b);
		  XSetForeground(d,gc,white);
		  XFillArcs(d,w,gc,arcb,c);
		  XSetForeground(d,gc,black);
		  XFillRectangle(d,w,gc,150,510,350,20);
		  XFillRectangle(d,w,gc,150,170,350,20);
		  XSetForeground(d,gc,white);
		  XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
			      strlen("ERROR. Press any key to exit."));
		  break;	  
		}
	      }
	    }
	  }
	}
      }else if(strcmp("ERROR",buf)==0){
	close(nsofd);
	printf("\t\tclient:%s\n",buf);
	printf("DISCONNECT\n");
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,150,510,350,20);
	XFillRectangle(d,w,gc,150,170,350,20);
	XSetForeground(d,gc,white);
	XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
		    strlen("ERROR. Press any key to exit."));
	while(1){
	  if(XPending(d)!=0){
	    XNextEvent(d,&event);
	    switch(event.type){
	    case KeyPress:
	      exit(0);
	    case Expose:
	      if(key%7==0){
		XSetForeground(d,gc,darkorange);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==1){
		XSetForeground(d,gc,darkgoldenrod);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==2){
		XSetForeground(d,gc,chocolate);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==3){
		XSetForeground(d,gc,coral);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==4){
		XSetForeground(d,gc,peru);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==5){
		XSetForeground(d,gc,sandybrown);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else{
		XSetForeground(d,gc,sienna);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }
	      XSetForeground(d,gc,black);
	      for(int i=150;i<=500;i+=50)
		XDrawLine(d,w,gc,i,200,i,500);
	      for(int j=200;j<=500;j+=50)
		XDrawLine(d,w,gc,150,j,500,j);
	      XSetForeground(d,gc,black);
	      XFillRectangle(d,w,gc,450,100,50,50);
	      XFillArcs(d,w,gc,arca,b);
	      XSetForeground(d,gc,white);
	      XFillArcs(d,w,gc,arcb,c);
	      XSetForeground(d,gc,black);
	      XFillRectangle(d,w,gc,150,510,350,20);
	      XFillRectangle(d,w,gc,150,170,350,20);
	      XSetForeground(d,gc,white);
	      XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
			  strlen("ERROR. Press any key to exit."));
	      break;	  
	    }
	  }
	}
      }else if(strcmp("YOU-WIN",buf)==0){
	close(nsofd);
	printf("\t\tclient:%s\n",buf);
	printf("DISCONNECT\n");
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,150,510,350,20);
	XFillRectangle(d,w,gc,150,170,350,20);
	XSetForeground(d,gc,white);
	XDrawString(d,w,gc,155,520,"You win! Press any key to exit.",
		    strlen("You win! Press any key to exit."));
	while(1){
	  if(XPending(d)!=0){
	    XNextEvent(d,&event);
	    switch(event.type){
	    case KeyPress:
	      exit(0);
	    case Expose:
	      if(key%7==0){
		XSetForeground(d,gc,darkorange);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==1){
		XSetForeground(d,gc,darkgoldenrod);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==2){
		XSetForeground(d,gc,chocolate);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==3){
		XSetForeground(d,gc,coral);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==4){
		XSetForeground(d,gc,peru);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==5){
		XSetForeground(d,gc,sandybrown);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else{
		XSetForeground(d,gc,sienna);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }
	      XSetForeground(d,gc,black);
	      for(int i=150;i<=500;i+=50)
		XDrawLine(d,w,gc,i,200,i,500);
	      for(int j=200;j<=500;j+=50)
		XDrawLine(d,w,gc,150,j,500,j);
	      XSetForeground(d,gc,black);
	      XFillRectangle(d,w,gc,450,100,50,50);
	      XFillArcs(d,w,gc,arca,b);
	      XSetForeground(d,gc,white);
	      XFillArcs(d,w,gc,arcb,c);
	      XSetForeground(d,gc,black);
	      XFillRectangle(d,w,gc,150,510,350,20);
	      XFillRectangle(d,w,gc,150,170,350,20);
	      XSetForeground(d,gc,white);
	      XDrawString(d,w,gc,155,520,"You win! Press any key to exit.",
			  strlen("You win! Press any key to exit."));
	      break;	  
	    }
	  }
	}
      }
      bzero(buf,BUFMAX);
    }
  }
}



int cliepro(){
  Display *d;
  Window w;
  XEvent event;
  GC gc;
  XGCValues gv;
  Colormap cmap;
  unsigned long MyColor();
  unsigned long darkorange,darkgoldenrod,chocolate,coral,peru,sandybrown,sienna,black,white;
  d=XOpenDisplay(NULL);
  darkorange=MyColor(d,"darkorange");
  darkgoldenrod=MyColor(d,"darkgoldenrod");
  chocolate=MyColor(d,"chocolate");
  coral=MyColor(d,"coral");
  peru=MyColor(d,"peru");
  sandybrown=MyColor(d,"sandybrown");
  sienna=MyColor(d,"sienna");
  black=MyColor(d,"black");
  white=MyColor(d,"white");
  
  tm.tv_sec=0;
  tm.tv_usec=1;

  printf("hostname?\n");
  scanf("%s",shostn);

  shost=gethostbyname(shostn);
  if(shost==NULL) Err("gethostbyname");

  bzero((char *)&svaddr, sizeof(svaddr));
  svaddr.sin_family=AF_INET;
  svaddr.sin_port=htons(PORT_NO);

  bcopy((char *)shost->h_addr,(char *)&svaddr.sin_addr,shost->h_length);

  sofd=socket(AF_INET,SOCK_STREAM,0);
  if(sofd<0) Err("socket");

  connect(sofd,(struct sockaddr*)&svaddr,sizeof(svaddr));
  write(1,"START\n",6);

    
  
  w=XCreateSimpleWindow(d,RootWindow(d,0),20,20,600,600,2,
			BlackPixel(d,DefaultScreen(d)),
			WhitePixel(d,DefaultScreen(d)));
  XSelectInput(d,w,ExposureMask|KeyPressMask|ButtonPressMask);
  XMapWindow(d,w);
  XStoreName(d,w,"Client : white");
  gv.line_width=1;
  gc=XCreateGC(d,w,GCLineWidth,&gv);
  XArc arca[500],arcb[500];
  int a=0,b=0,c=0;
  int flag=0;
  int key=0;
  char count[100];
    
  while(1){
    FD_ZERO(&mask);
    FD_SET(sofd,&mask);
    FD_SET(0,&mask);
    retval=select(sofd+1,&mask,NULL,NULL,&tm);

    if(XPending(d)!=0){
      XNextEvent(d,&event);
      switch(event.type){
      case Expose:
	if(key%7==0){
	  XSetForeground(d,gc,darkorange);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==1){
	  XSetForeground(d,gc,darkgoldenrod);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==2){
	  XSetForeground(d,gc,chocolate);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==3){
	  XSetForeground(d,gc,coral);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==4){
	  XSetForeground(d,gc,peru);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else if(key%7==5){
	  XSetForeground(d,gc,sandybrown);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}else{
	  XSetForeground(d,gc,sienna);
	  XFillRectangle(d,w,gc,150,200,350,300);
	}
	XSetForeground(d,gc,black);
	for(int i=150;i<=500;i+=50)
	  XDrawLine(d,w,gc,i,200,i,500);
	for(int j=200;j<=500;j+=50)
	  XDrawLine(d,w,gc,150,j,500,j);
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,450,100,50,50);
	XFillArcs(d,w,gc,arca,b);
	XSetForeground(d,gc,white);
	XFillArcs(d,w,gc,arcb,c);
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,150,510,350,20);
	XFillRectangle(d,w,gc,150,170,350,20);
	XSetForeground(d,gc,white);
	XDrawString(d,w,gc,155,520,"You have just started the game",
		    strlen("You have just started the game"));
	XDrawString(d,w,gc,155,528,"(or your windowsettings have just been changed).",
		    strlen("(or your windowsettings have just been changed)."));
	sprintf(count,"Black stones : %d , White stones : %d",b,c);
	XDrawString(d,w,gc,225,180,count,strlen(count));
	break;

	
      case ButtonPress:
	if(event.xbutton.x<500 && event.xbutton.x>150 && event.xbutton.y<500 && event.xbutton.y>200){
	  if(((event.xbutton.y/50-4)==5||map[event.xbutton.x/50-3][event.xbutton.y/50-3]!=0)&&c==b-1){
	     for(int p=0;p<b;p++){
	       if(arca[p].x==50*(event.xbutton.x/50) && arca[p].y==50*(event.xbutton.y/50)){
		 flag=1;
		 XSetForeground(d,gc,black);
		 XFillRectangle(d,w,gc,150,510,350,20);
		 XSetForeground(d,gc,white);
		 XDrawString(d,w,gc,155,520,"That's where server has already placed the stones!",
			     strlen("That's where server has already placed the stones!"));
		 break;
	       }
	     }
	     for(int p=0;p<c;p++){
	       if(arcb[p].x==50*(event.xbutton.x/50) && arcb[p].y==50*(event.xbutton.y/50)){
		 flag=1;
		 XSetForeground(d,gc,black);
		 XFillRectangle(d,w,gc,150,510,350,20);
		 XSetForeground(d,gc,white);
		 XDrawString(d,w,gc,155,520,"That's where you've already placed the stones!",
			     strlen("That's where you've already placed the stones!"));
		 break;
	       }
	     }
	     if(flag){
	       flag=0;
	       break;
	     }
	     arcb[c].x=50*(event.xbutton.x/50);
	     arcb[c].y=50*(event.xbutton.y/50);
	     arcb[c].width=50;
	     arcb[c].height=50;
	     arcb[c].angle1=0;
	     arcb[c].angle2=360*64;
	     XSetForeground(d,gc,white);
	     map[event.xbutton.x/50-3][event.xbutton.y/50-4]=2;
	     XFillArc(d,w,gc,arcb[c].x,arcb[c].y,50,50,0,360*64);
	     c++;
	     
	     sprintf(buf,"PLACE-%d%d",event.xbutton.x/50-3,event.xbutton.y/50-4);
	     n=sizeof(buf);
	     write(sofd,buf,n);
	     printf("you:%s\n",buf);
	     
	     XSetForeground(d,gc,black);
	     XFillRectangle(d,w,gc,150,510,350,20);
	     XFillRectangle(d,w,gc,150,170,350,20);
	     XSetForeground(d,gc,white);
	     XDrawString(d,w,gc,155,520,"You have placed a white stone.",
			 strlen("You have placed a white stone."));
	     sprintf(count,"Black stones : %d , White stones : %d",b,c);
	     XDrawString(d,w,gc,225,180,count,strlen(count));
	  }
	}else if(event.xbutton.x<=500 && event.xbutton.x>=450 && event.xbutton.y<=150 && event.xbutton.y>=100){
	  key++;
	  if(key%7==0){
	    XSetForeground(d,gc,darkorange);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==1){
	    XSetForeground(d,gc,darkgoldenrod);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==2){
	    XSetForeground(d,gc,chocolate);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==3){
	    XSetForeground(d,gc,coral);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==4){
	    XSetForeground(d,gc,peru);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else if(key%7==5){
	    XSetForeground(d,gc,sandybrown);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }else{
	    XSetForeground(d,gc,sienna);
	    XFillRectangle(d,w,gc,150,200,350,300);
	  }
	  XSetForeground(d,gc,black);
	  for(int i=150;i<=500;i+=50)
	    XDrawLine(d,w,gc,i,200,i,500);
	  for(int j=200;j<=500;j+=50)
	    XDrawLine(d,w,gc,150,j,500,j);
	  XFillArcs(d,w,gc,arca,b);
	  XSetForeground(d,gc,white);
	  XFillArcs(d,w,gc,arcb,c);
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"You've changed the color of board.",
		      strlen("You've changed the color of board."));
	  break;
	}else{
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"You're clicking out of range!",
		      strlen("You're clicking out of range!"));
	}
	break;
      }
      bzero(buf,BUFMAX);
    }

    if(FD_ISSET(sofd,&mask)){
      n=read(sofd,buf,BUFMAX);
      if(buf[0]=='P'&&buf[1]=='L'&&buf[2]=='A'&&buf[3]=='C'&&buf[4]=='E'&&buf[5]=='-'&&buf[6]>='0'&&buf[6]<='6'&&buf[7]>='0'&&buf[7]<='5'){
	char tmp1[10],tmp2[10];
	sprintf(tmp1,"%c",buf[6]);
	sprintf(tmp2,"%c",buf[7]);
	for(int p=0;p<b;p++){
	  if(arca[p].x==150+50*atoi(tmp1) && arca[p].y==200+50*atoi(tmp2)){
	    flag=1;
	    break;
	  }
	}
	for(int p=0;p<c;p++){
	  if(arcb[p].x==150+50*atoi(tmp1) && arcb[p].y==200+50*atoi(tmp2)){
	    flag=1;
	    break;
	  }
	}
	if(flag==1||b==c+1||(map[atoi(tmp1)][atoi(tmp2)+1]==0&&atoi(tmp2)!=5)){
	  flag=0;
	  printf("\t\tserver:%s\n",buf);
	  sprintf(buf,"ERROR");
	  n=sizeof(buf);
	  write(sofd,buf,n);
	  close(sofd);
	  printf("you:%s\n",buf);
	  printf("DISCONNECT\n");
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XFillRectangle(d,w,gc,150,170,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
		      strlen("ERROR. Press any key to exit."));
	  while(1){
	    if(XPending(d)!=0){
	      XNextEvent(d,&event);
	      switch(event.type){
	      case KeyPress:
		exit(0);
	      case Expose:
		if(key%7==0){
		  XSetForeground(d,gc,darkorange);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==1){
		  XSetForeground(d,gc,darkgoldenrod);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==2){
		  XSetForeground(d,gc,chocolate);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==3){
		  XSetForeground(d,gc,coral);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==4){
		  XSetForeground(d,gc,peru);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==5){
		  XSetForeground(d,gc,sandybrown);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else{
		  XSetForeground(d,gc,sienna);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}
		XSetForeground(d,gc,black);
		for(int i=150;i<=500;i+=50)
		  XDrawLine(d,w,gc,i,200,i,500);
		for(int j=200;j<=500;j+=50)
		  XDrawLine(d,w,gc,150,j,500,j);
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,450,100,50,50);
		XFillArcs(d,w,gc,arca,b);
		XSetForeground(d,gc,white);
		XFillArcs(d,w,gc,arcb,c);
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,150,510,350,20);
		XFillRectangle(d,w,gc,150,170,350,20);
		XSetForeground(d,gc,white);
		XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
			    strlen("ERROR. Press any key to exit."));
		break;	  
	      }
	    }
	  }
	}
	else{
	  arca[b].x=150+50*atoi(tmp1);
	  arca[b].y=200+50*atoi(tmp2);
	  arca[b].width=50;
	  arca[b].height=50;
	  arca[b].angle1=0;
	  arca[b].angle2=360*64;
	  XSetForeground(d,gc,black);
	  map[atoi(tmp1)][atoi(tmp2)]=1;
	  XFillArc(d,w,gc,arca[b].x,arca[b].y,50,50,0,360*64);
	  b++;
	  XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XFillRectangle(d,w,gc,150,170,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"Now your turn.",
		      strlen("Now your turn."));
	  sprintf(count,"Black stones : %d , White stones : %d",b,c);
	  XDrawString(d,w,gc,225,180,count,strlen(count));
	  printf("\t\tserver:%s\n",buf);
	  int x=vertical(),y=horizontal(),z=diagonal();
	  if(x==1||y==1||z==1){
	    sprintf(buf,"YOU-WIN");
	    n=sizeof(buf);
	    write(sofd,buf,n);
	    printf("YOU-LOSE\n");
	    close(sofd);
	    printf("DISCONNECT\n");
	    XSetForeground(d,gc,black);
	    XFillRectangle(d,w,gc,150,510,350,20);
	    XFillRectangle(d,w,gc,150,170,350,20);
	    XSetForeground(d,gc,white);
	    XDrawString(d,w,gc,155,520,"You lose. Press any key to exit.",
			strlen("You lose. Press any key to exit."));
	    while(1){
	      if(XPending(d)!=0){
		XNextEvent(d,&event);
		switch(event.type){
		case KeyPress:
		  exit(0);
		case Expose:
		  if(key%7==0){
		    XSetForeground(d,gc,darkorange);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==1){
		    XSetForeground(d,gc,darkgoldenrod);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==2){
		    XSetForeground(d,gc,chocolate);
		  XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==3){
		    XSetForeground(d,gc,coral);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==4){
		    XSetForeground(d,gc,peru);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else if(key%7==5){
		    XSetForeground(d,gc,sandybrown);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }else{
		    XSetForeground(d,gc,sienna);
		    XFillRectangle(d,w,gc,150,200,350,300);
		  }
		  XSetForeground(d,gc,black);
		  for(int i=150;i<=500;i+=50)
		    XDrawLine(d,w,gc,i,200,i,500);
		  for(int j=200;j<=500;j+=50)
		    XDrawLine(d,w,gc,150,j,500,j);
		  XSetForeground(d,gc,black);
		  XFillRectangle(d,w,gc,450,100,50,50);
		  XFillArcs(d,w,gc,arca,b);
		  XSetForeground(d,gc,white);
		  XFillArcs(d,w,gc,arcb,c);
		  XSetForeground(d,gc,black);
		  XFillRectangle(d,w,gc,150,510,350,20);
		  XFillRectangle(d,w,gc,150,170,350,20);
		  XSetForeground(d,gc,white);
		  XDrawString(d,w,gc,155,520,"You lose. Press any key to exit.",
			      strlen("You lose. Press any key to exit."));
		  break;	  
		}
	      }
	    }
	  }
	}
      }else if(strcmp("ERROR",buf)==0){
	close(sofd);
	printf("\t\tserver:%s\n",buf);
	printf("DISCONNECT\n");
	XSetForeground(d,gc,black);
	XFillRectangle(d,w,gc,150,510,350,20);
	XFillRectangle(d,w,gc,150,170,350,20);
	XSetForeground(d,gc,white);
	XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
		    strlen("ERROR. Press any key to exit."));
	while(1){
	  if(XPending(d)!=0){
	    XNextEvent(d,&event);
	    switch(event.type){
	    case KeyPress:
	      exit(0);
	    case Expose:
	      if(key%7==0){
		XSetForeground(d,gc,darkorange);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==1){
		XSetForeground(d,gc,darkgoldenrod);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==2){
		XSetForeground(d,gc,chocolate);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==3){
		XSetForeground(d,gc,coral);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==4){
		XSetForeground(d,gc,peru);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else if(key%7==5){
		XSetForeground(d,gc,sandybrown);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }else{
		XSetForeground(d,gc,sienna);
		XFillRectangle(d,w,gc,150,200,350,300);
	      }
	      XSetForeground(d,gc,black);
	      for(int i=150;i<=500;i+=50)
		XDrawLine(d,w,gc,i,200,i,500);
	      for(int j=200;j<=500;j+=50)
		XDrawLine(d,w,gc,150,j,500,j);
	      XSetForeground(d,gc,black);
	      XFillRectangle(d,w,gc,450,100,50,50);
	      XFillArcs(d,w,gc,arca,b);
	      XSetForeground(d,gc,white);
	      XFillArcs(d,w,gc,arcb,c);
	      XSetForeground(d,gc,black);
	      XFillRectangle(d,w,gc,150,510,350,20);
	      XFillRectangle(d,w,gc,150,170,350,20);
	      XSetForeground(d,gc,white);
	      XDrawString(d,w,gc,155,520,"ERROR. Press any key to exit.",
			  strlen("ERROR. Press any key to exit."));
	      break;
	    }
	  }
	}
      }else if(strcmp("YOU-WIN",buf)==0){
	close(sofd);
	printf("\t\tserver:%s\n",buf);
	printf("DISCONNECT\n");
	XSetForeground(d,gc,black);
	  XFillRectangle(d,w,gc,150,510,350,20);
	  XFillRectangle(d,w,gc,150,170,350,20);
	  XSetForeground(d,gc,white);
	  XDrawString(d,w,gc,155,520,"You win! Press any key to exit.",
		      strlen("You win! Press any key to exit."));
	  while(1){
	    if(XPending(d)!=0){
	      XNextEvent(d,&event);
	      switch(event.type){
	      case KeyPress:
		exit(0);
	      case Expose:
		if(key%7==0){
		  XSetForeground(d,gc,darkorange);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==1){
		  XSetForeground(d,gc,darkgoldenrod);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==2){
		  XSetForeground(d,gc,chocolate);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==3){
		  XSetForeground(d,gc,coral);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==4){
		  XSetForeground(d,gc,peru);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else if(key%7==5){
		  XSetForeground(d,gc,sandybrown);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}else{
		  XSetForeground(d,gc,sienna);
		  XFillRectangle(d,w,gc,150,200,350,300);
		}
		XSetForeground(d,gc,black);
		for(int i=150;i<=500;i+=50)
		  XDrawLine(d,w,gc,i,200,i,500);
		for(int j=200;j<=500;j+=50)
		  XDrawLine(d,w,gc,150,j,500,j);
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,450,100,50,50);
		XFillArcs(d,w,gc,arca,b);
		XSetForeground(d,gc,white);
		XFillArcs(d,w,gc,arcb,c);
		XSetForeground(d,gc,black);
		XFillRectangle(d,w,gc,150,510,350,20);
		XFillRectangle(d,w,gc,150,170,350,20);
		XSetForeground(d,gc,white);
		XDrawString(d,w,gc,155,520,"You win! Press any key to exit.",
			    strlen("You win! Press any key to exit."));
		break;
	      }
	    }
	  }
      }
      bzero(buf,BUFMAX);
    }
  }
}


int vertical(){
  int serv,clie;

  for(int i=0;i<7;i++){
    serv=0;
    clie=0;
    for(int j=0;j<6;j++){
      
      if(map[i][j]==1){
	serv+=1;
	clie=0;
      }else if(map[i][j]==2){
	serv=0;
	clie+=1;
      }else{
	serv=0;
	clie=0;
      }

      if(serv==4){
	return 1;
      }else if(clie==4){
	return 2;
      }
      
    }
  }

  return 0;
  
}

int horizontal(){
  int serv,clie;

  for(int j=0;j<6;j++){
    serv=0;
    clie=0;
    for(int i=0;i<7;i++){

      if(map[i][j]==1){
	serv+=1;
	clie=0;
      }else if(map[i][j]==2){
	serv=0;
	clie+=1;
      }else{
	serv=0;
	clie=0;
      }
      if(serv==4){
	return 1;
      }else if(clie==4){
	return 2;
      }
    }
  }

  return 0;
  
}


int diagonal(){
  int serv,clie;
  for(int i=0;i<7;i++){
    serv=0;
    clie=0;
    for(int j=0,k=0;j<7,k<6;j++,k++){
      int point=map[i+j][k];

      if(point==1){
	serv++;
	clie=0;
      }else if(point==2){
	serv=0;
	clie++;
      }else{
	serv=0;
	clie=0;
      }

      if(serv==4){
	return 1;
      }else if(clie==4){
	return 2;
      }
      
    }
  }

  serv=0;
  clie=0;

  for(int i=6;i>=0;i--){
    serv=0;
    clie=0;
    for(int j=0,k=0;j<7,k<6;j++,k++){
      int point=map[i-k][j];
    
      if(point==1){
	serv++;
	clie=0;
      }else if(point==2){
	serv=0;
	clie++;
      }else{
	serv=0;
	clie=0;
      }

      if(serv==4){
	return 1;
      }else if(clie==4){
	return 2;
      }
      
    }
  }

  for(int i=1;i<3;i++){
    serv=0;
    clie=0;
    for(int j=0,k=0;j<5,k<6;j++,k++){
      int point = map[j][i+k];

      if(point==1){
	serv++;
	clie=0;
      }else if(point==2){
	serv=0;
	clie++;
      }else{
	serv=0;
	clie=0;
      }

      if(serv==4){
	return 1;
      }else if(clie==4){
	return 2;
      }
      
    }
  }

  for(int i=5;i<7;i++){
    serv=0;
    clie=0;
    for(int j=2,k=0;j<7,k<5;j++,k++){
      int point=map[j][i-k];

      if(point==1){
	serv++;
	clie=0;
      }else if(point==2){
	serv=0;
	clie++;
      }else{
	serv=0;
	clie=0;
      }

      if(serv==4){
	return 1;
      }else if(clie==4){
	return 2;
      }

      
    }
  }
  return 0;
}
    
