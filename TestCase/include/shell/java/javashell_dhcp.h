#ifndef JAVASHELL_DHCP_H
#define JAVASHELL_DHCP_H

#ifdef __cplusplus
extern "C" {
#endif


void CSDhcpSetOptions(char * pcEthName);
void CSDhcpSetServerName(char * pcEthName);


char* CSShellDhcpGetClientClassID();
char* CSShellDhcpGetServerClassID();

#ifdef __cplusplus
}
#endif

#endif  /* JAVASHELL_DHCP_H */

