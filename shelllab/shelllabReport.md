PK
    qtNïòÖé!  !    ) CSAPP : shelllab å®éªæ¥å.mdup% ¿CSAPP : shelllab å®éªæ¥å.md# CSAPP : shelllab å®éªæ¥å

å§åï¼æ½æ¥å³°

å­¦å·ï¼PB18000335


## å®éªä»ç»

--------------------

é¢ç®è¦æ±æä»¬å®æä¸ä¸ªç®åç Linux Shellï¼å¶ä¸­å¤§é¨åå½æ°å·²ç»åå¥½ï¼æä»¬åªéå®æå©ä½çä¸é¨åç¨åºãæ¶åå°ç¬¬å«ç« å¼å¸¸æ§å¶æµä¸­ä¿¡å·æ¥æ¶å¤çç¸å³çåå®¹ã

## å®éªåå¤

--------------------
ä¸å®è¦å¯¹ä¹¦æ¬ä¸çåå®¹ååçè§£ï¼ä¸ç¶å lab çæ¶åå°±ä¼æè¶³æ æªï¼ä¸å®è¦è¯¦ç»äºè§£åä¸ªä¿¡å·ç¸å³çå½æ°åæ°çå«ä¹ï¼ä»¥åè¿åå¼çæä¹ã

### å¾å®æå½æ°

å½æ°å|åè½|
:-:|:-:|
void eval(char *cmdline);|å¤çè¾å¥å½ä»¤è¡
int builtin_cmd(char **argv);|æ§è¡åç½®å½ä»¤ï¼å¦æä¸æ¯åç½®å½ä»¤åè¿å 0
void do_bgfg(char **argv);|æ§è¡åç½®ç bg fg å½ä»¤
void waitfg(pid_t pid);|æ¾å¼å°ç­å¾è¿ç¨ç»æ
void sigchld_handler(int sig);|å¤ç SIGCHLD ä¿¡å·
void sigtstp_handler(int sig);|å¤ç SIGTSTP ä¿¡å·
void sigint_handler(int sig);|å¤ç SIGINT ä¿¡å·

### å½æ°ååæ°è¯´æ

#### waitpid

```
	pid_t waitpid(pid_t pid,int * status,int options);
```
åæ° options æä¾äºä¸äºé¢å¤çéé¡¹æ¥æ§å¶ waitpid
* WNOHANG å¦æç­å¾éåä¸­çä»»ä½å­è¿ç¨é½è¿æ²¡æç»æ­¢ï¼é£ä¹å°±ç«å³è¿å 0ã
* WUNTRACED æèµ·è°ç¨è¿ç¨çæ§è¡ï¼ç´å°ç­å¾éåä¸­çä¸ä¸ªè¿ç¨åæå·²ç»æ­¢æèè¢«åæ­¢ã
* WCONTINUED æèµ·è°ç¨è¿ç¨çæ§è¡ï¼ç´å°ç­å¾éåä¸­ä¸ä¸ªæ­£å¨è¿è¡çè¿ç¨ç»æ­¢æéåä¸­ä¸ä¸ªè¢«åæ­¢çè¿ç¨æ¶å° SIGCONT ä¿¡å·éæ°å¼å§æ§è¡ã
* WNOHANG | WUNTRACED ç«å³è¿åï¼å¦æç­å¾éåä¸­çå­è¿ç¨é½æ²¡æè¢«åæ­¢æç»æ­¢ï¼åè¿å 0ï¼å¦ææä¸ä¸ªåæ­¢æç»æ­¢ï¼åè¿åå­è¿ç¨ç PIDã

å¤æ­è¿ç¨éåºç¶æ status çå®

å®å|åè½|
:-:|:-:|
WIFEXITED(status)|å¦æå­è¿ç¨æ­£å¸¸ç»æ­¢åä¸ºé0å¼
WEXITSTATUS(status)|åå¾å­è¿ç¨ exit() çè¿åå¼ï¼åªæ WIFEXITED() è¿åä¸ºçæ¶æå®ä¹è¿ä¸ªç¶æ
WIFSIGNALED(status)|å­è¿ç¨å ä¿¡å·ç»æ­¢æ¶å®å¼ä¸ºç
WTERMSIG(status)|è¿åå¯¼è´å­è¿ç¨ç»æ­¢çä¿¡å·ç¼å·ï¼åªæ WIFSIGNALED() è¿åä¸ºçæ¶ï¼æå®ä¹è¿ä¸ªç¶æ
WIFSTOPPED(status)|å­è¿ç¨å¤äºåæ­¢ç¶ææ¶å®å¼ä¸ºç
WSTOPSIG(status)|è¿åå¯¼è´å­è¿ç¨åæ­¢çä¿¡å·ç¼å·ï¼åªæ WIFSTOPPED() è¿åä¸ºçæ¶ï¼æå®ä¹è¿ä¸ªç¶æ

#### kill

```
	int kill(pid_t pid,int sig);
```
kill() å¯ä»¥ç¨æ¥éåæ° sig æå®çä¿¡å·ç»åæ° pid æå®çè¿ç¨ãåæ° pid æå ç§æåµ
* pid > 0 å°ä¿¡å·ä¼ ç»è¿ç¨è¯å«ç ä¸º pid çè¿ç¨ã
* pid = 0 å°ä¿¡å·ä¼ ç»åç®åè¿ç¨ç¸åè¿ç¨ç»çææè¿ç¨
* pid = -1 å°ä¿¡å·å¹¿æ­ä¼ éç»ç³»ç»åææçè¿ç¨
* pid < 0 å°ä¿¡å·ä¼ ç»è¿ç¨ç»è¯å«ç ä¸º pid ç»å¯¹å¼çææè¿ç¨

æ§è¡æååè¿å 0 ,å¦ææéè¯¯åè¿å -1 ã

#### é»å¡åè§£é¤é»å¡ä¿¡å·

å½æ°å|åè½|
:-:|:-:|
int sigemptyset(sigset_t* set);|åå§åä¿¡å·éï¼æ¸ç©ºï¼
int sigfillset(sigset_t* set);|åå§åä¿¡å·éï¼æå®ä½ç½® 1ï¼
int sigaddset(sigset_t* set,int signo);|æ·»å ææä¿¡å· signo
int sigdelset(sigset_t* set,int signo);|å é¤ææä¿¡å· signo
int sigismember(const sigset_t* set,int signo);|å¤æ­ä¿¡å·éä¸­çææä¿¡å·æ¯å¦åå«ç¼å·ä¸º signo çä¿¡å·
int sigprocmask(int how,const sigset_t* set,sigset_t* oset);|è¯»åææ´æ¹è¿ç¨çé»å¡ä¿¡å·é

**åæ°è¯´æ**

how

å¦ä½æä½ä¿¡å·å±è½å­ï¼æ­¤åæ°æä¸ä¸ªå¯éå¼ï¼
* SIG_BLOCK é»å¡è¯¥ä¿¡å·
* SIG_UNBLOCK è§£é¤ä¿¡å·é»å¡
* SIG_SETMASK è®¾ç½®å½åä¿¡å·å±è½å­ä¸ºsetææåçå¼

set/oset

* å¦æ oset æ¯éç©ºæéï¼åè¯»åè¿ç¨çå½åä¿¡å·å±è½å­éè¿osetåæ°ä¼ åº
* å¦æ set æ¯éç©ºæéï¼åæ´æ¹å½åè¿ç¨çä¿¡å·å±è½å­

## å®éªè¿ç¨

--------------------
åä¸æç¨åºçæ¶åçäºä¸ä¸åç½®ç helper routines åç°å¥½åå­å¨ä¸äº bug ï¼å®å¯¹ `jobid` çåéå­å¨ä¸äºé®é¢ã

![1.jpg](https://raw.githubusercontent.com/Afool1999/CSAPP-Labs/master/shelllab/pic/1.jpg)
å¾ä¸­ä¸¤ä¸ªä¸å `job` ç `jobid` åä¸º `1`

### eval

```
	void eval(char *cmdline)
	{
		char *argv[MAXARGS];
		char buf[MAXLINE];
		int bg;
		pid_t pid = 0;
	
		strcpy(buf, cmdline);
		bg = parseline(buf, argv);
		if (argv[0] == NULL)
			return;
	
		sigset_t mask_all, mask_one, prev_one;
	
		sigfillset(&mask_all);
		sigemptyset(&mask_one);
		sigaddset(&mask_one, SIGCHLD);
	
		if (builtin_cmd(argv) == 0)
		{
			sigprocmask(SIG_BLOCK, &mask_one, &prev_one);
			if ((pid = fork()) == 0)
			{
				setpgid(0, 0);
				sigprocmask(SIG_SETMASK, &prev_one, NULL);
	
				if (execve(argv[0], argv, environ) == -1) {
					printf("%s: Command not found\n", argv[0]);
					_exit(0);
				}
			}
			sigprocmask(SIG_BLOCK, &mask_all, NULL);
	
			if (bg == 1) addjob(jobs, pid, BG, cmdline);
				else addjob(jobs, pid, FG, cmdline);
	
			sigprocmask(SIG_SETMASK, &prev_one, NULL);
		}
		else return;
	
		if (bg == 1) printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
			else waitfg(pid);
	
		return;
	}
```

ä¹¦æ¬ä¸æç±»ä¼¼çä»£ç ï¼ä¸ºäºé¿åç«äºå¼èµ·éè¯¯ï¼è¦å¨ addjob åé»å¡ä¿¡å·ï¼åæ¶ç±äºå­è¿ç¨ç»§æ¿ç¶è¿ç¨çä¿¡å·é»å¡ï¼æä»¥è¦å¨ execve åè§£é¤å¯¹ä¿¡å·çé»å¡ãå©ç¨ setpgid è®¾ç½®å­è¿ç¨æå±å·¥ä½ç»ï¼ä»¥ä¿è¯å¨ shell çåå°è¿ç¨ç»ä¸­åªæèªå·±ç tsh è¿ç¨ã

### builtin_cmd

```
	int builtin_cmd(char **argv)
	{
		if (strcmp(argv[0], "quit") == 0)
		{
			_exit(0);
		}
		else if (strcmp(argv[0], "jobs") == 0)
		{
			listjobs(jobs);
		}
		else if (strcmp(argv[0], "bg") == 0)
		{
			do_bgfg(argv);
		}
		else if (strcmp(argv[0], "fg") == 0)
		{
			do_bgfg(argv);
		}
		else return 0;		/* not a builtin command */
		
		return 1;     
	}
```
ä»ä»æ¯å¯¹å½ä»¤çè§£æä¸å½æ°è°ç¨ã

### do_bgfg

```
	void do_bgfg(char **argv)
	{
		if (argv[1] == NULL)
		{
			printf("%s command requires PID or %%jobid argument\n", argv[0]);
			return;
		}
	
		int isjid, id;
		char *buf, *stop;
		isjid = 0;
		buf = argv[1];
	
		if (*buf == '%')
		{
			isjid = 1;
			++buf;
		}
		id = strtol(buf, &stop, 10);
		if (buf == stop)
		{
			printf("%s bg: argument must be a PID or %%jobid\n", argv[0]);
			return;
		}
	
		struct job_t *job;
		if (isjid == 1) job = getjobjid(jobs, id);
			else job = getjobpid(jobs, id);
		if (job == NULL)
		{
			if (isjid == 1) printf("%s: No such job\n", argv[1]);
				else printf("(%d): No such process\n", id);
			return;
		}
	
		int pid, jid;
		pid = job->pid;
		jid = job->jid;
		if (strcmp(argv[0], "fg") == 0)
		{
			job->state = FG;
			kill(-pid, SIGCONT);
			waitfg(pid);
		}
		else
		{
			job->state = BG;
			kill(-pid, SIGCONT);
			printf("[%d] (%d) %s", jid, pid, job->cmdline);
		}
	
		return;
	}
```
è¦ç¡®ä¿ä¼ å¥åæ°çåæ³æ§ãåèç¨åºè¾åºä¿¡æ¯ä¸ä¸è´ççå¾è®©äººå¤´ç¼ã
![2.jpg](https://raw.githubusercontent.com/Afool1999/CSAPP-Labs/master/shelllab/pic/2.jpg)

### waitfg

```
	void waitfg(pid_t pid)
	{
		while (fgpid(jobs) == pid)
			sleep(1);
	
		return;
	}
```
ä½¿ç¨ busy_loop ï¼handler ä¸­æ´æ¹å¨å±åéï¼ä¸æ¦åå°è¿ç¨ç»æï¼å°±æ²¡æä»»ä½ä¸ä¸ªåå°è¿ç¨ï¼äºæ¯åæ­¢ç­å¾ã

## å®éªæ»ç»
------------------

å¶å®è¿ä¸ªå®éªè¿åºè¯¥å³æ³¨ä¸ä¸å¯¹äºå½æ°è¿åå¼çå¤çï¼ä»¥åå¹¶åéè¯¯çå¤çç­æ¹é¢ï¼æ­£å¦ä¹¦ä¸æåæ¶åå°å¨å±åéçå½æ°é½æ¯ä¸å®å¨çï¼è¿æ¬¡å®éªä¹æ²¡è½åæä¸ä¸ªâçæ­£âå®å¨çï¼ä¸è¿å¦æçèèèµ·å®å¨æ§æ¥ï¼æè§æ¯ä¸ªè¯­å¥åé½æ¯ä¸å®å¨çäºãä¸è¿éè¿è¿ä¸ªå®éªï¼è®©æå¯¹ä¿¡å·çå¤çæäºæ´ç´è§çäºè§£ãå¦ä½åå»ºæ°è¿ç¨ï¼å¦ä½å¨è¿ç¨ä¸­æ§è¡å¦ä¸ä¸ªç¨åºï¼ç¨åºçä¿¡å·å¦ä½è¢«ä¼ éï¼æ¥æ¶å°ä¿¡å·åå¦ä½è¿ä¸æ­¥å¤çï¼è¿äºç¥è¯ç¹å¨è¿ä¸æ¬¡å®éªä¸­é½æä½ç°ã

ä¸è¿è¿æ¬¡å®éªéå°äºä¸ä¸ªé®é¢ï¼å¨ä½¿ç¨ Ctrl+Z æååå°è¿ç¨æ¶ï¼å´è¿å stopped by signal 127ï¼ç½ä¸ç»åºçè¯´æ³æ¯

1 å¦æforkå¤±è´¥äºï¼æèwaitpidè¿åé¤äºEINTRä¹å¤çéè¯¯ï¼systemè¿å -1ï¼
2 execlæ§è¡å¤±è´¥ï¼å¶è¿åå¼å¦åshellæ§è¡äº"exit(127)" ä¸æ ·ã
3 å¦æä¸è¿°ä¸æ­¥é½æ§è¡æåï¼é£ä¹ï¼systemè¿åå¼æ¯shellçç»æ­¢ç¶æã

ä¸è¿è¿æ¯æä¸æ¸ä¸ºä»ä¹ã

![3.jpg](https://raw.githubusercontent.com/Afool1999/CSAPP-Labs/master/shelllab/pic/3.jpg)

## å¶å®
------------------

æ¬å®éªææèµæºå¨ [CSAPP-Labs/shelllab at master Â· Afool1999/CSAPP-Labs Â· GitHub](https://github.com/Afool1999/CSAPP-Labs/tree/master/shelllab)PK 
    qtNïòÖé!  !    )               CSAPP : shelllab å®éªæ¥å.mdup% ¿CSAPP : shelllab å®éªæ¥å.mdPK      w   |!    