#include "_string.h"

// Converts C-string to uppercase until zero or space is found.
// Returns pointer to the zero or space.
uint8_t* str_toupper(uint8_t *c){
	while (*c && (*c != ' ')){
		if ((*c >= 97) && (*c <= 122)) *c -= 32;
		c++;
	}
	return c;
}

// Destination is a put-function (rather than a uint8_tacter array pointer)
uint8_t str_utoap(void (*put)(uint8_t), uint32_t n){
	uint8_t str[11]; // 10 digits plus termination
	uint8_t len = 0;
	uint8_t j;

	do{
		str[len++] = (n % 10) + '0';
		n /= 10;
	}while(n);
	for (j=0; j<len; j++) put(str[len-1-j]);
	return len;
}

// Destination is a put-function (rather than a uint8_tacter array pointer)
uint8_t str_itoap(void (*put)(uint8_t), int32_t n){
	uint8_t str[12]; // Sign, 10 digits, termination
	uint8_t len = 0;
	uint8_t j;

	if (n<0){
		put('-');
		n *= -1;
	}
	do{
		str[len++] = (n % 10) + '0';
		n /= 10;
	}while(n);
	for (j=0; j<len; j++) put(str[len-1-j]);
	return len;
}

// Destination is a put-function (rather than a uint8_tacter array pointer)
uint8_t str_cpyp(void (*put)(uint8_t), uint8_t *src){
    int i;
	for (i=0; src[i]; i++) put(src[i]);
    return i;
}

// String copy with const source and put-function
uint8_t str_cpycp(void (*put)(uint8_t), const uint8_t *src){
    int i;
	for (i=0; src[i]; i++) put(src[i]);
    return i;
}

void str_htoap(void (*put)(uint8_t), uint32_t n, uint8_t n_len){
	uint8_t j;
	uint8_t temp;
	uint8_t bitshift = (n_len-1)*4;
	uint32_t mask = 0xF << bitshift;
	for (j=0; j<n_len; j++){
		temp = (n & mask) >> bitshift;
		n <<= 4;
		if(temp < 10) put(temp + '0');
		else put(temp + 55);
	}
}

void str_cpy(uint8_t* dest, const uint8_t *src){
    while (*src) *dest++ = *src++;
	*dest = 0;	// Added zero terminal
}

// Destination is pointer with length
void str_cpycl(uint8_t* d, uint16_t* d_len, uint16_t d_size, const uint8_t *src){
    int j;
	for (j=0; src[j] && ((*d_len)<d_size); j++) d[(*d_len)++] = src[j];
}

// Source and destination has length
void str_cpyll(uint8_t* d, uint16_t* d_len, uint16_t d_size, uint8_t *src, uint16_t src_len){
    int j;
	for (j=0; (j<src_len) && ((*d_len)<d_size); j++) d[(*d_len)++] = src[j];
}

void str_utoal(uint8_t* d, uint16_t* d_len, uint16_t d_size, uint32_t n){
	uint8_t str[10]; // 10 digits
	uint8_t j, len = 0;

	do{
		str[len++] = (n % 10) + '0';
		n /= 10;
	}while(n);
	for (j=0; (j<len)&&((*d_len)<d_size); j++) d[(*d_len)++] = str[len-1-j];
}

// Destination is a put-function (rather than a uint8_tacter array pointer)
void str_itoal(uint8_t* d, uint16_t* d_len, uint16_t d_size, int32_t n){
	uint8_t str[10]; // Sign, 10 digits
	uint8_t len = 0;
	uint8_t j;

    if ((*d_len)>=d_size) return;
	if (n<0){
		d[(*d_len)++] = '-';
		n *= -1;
	}
	do{
		str[len++] = (n % 10) + '0';
		n /= 10;
	}while(n);
	for (j=0; (j<len)&&((*d_len)<d_size); j++) d[(*d_len)++] = str[len-1-j];
}

// Destination, destination length, destination buffer size, number, decimal point
// Decimal integer to ascii with length
void str_ditoal(uint8_t* d, uint16_t* d_len, uint16_t d_size, int32_t n, uint8_t np){
    #define S_SIZE 10
	uint8_t s[S_SIZE]; // Sign, 10 digits
	uint8_t s_len = 0;
	uint8_t j;

    // Check sign
    if (n<0){
		if (*d_len<d_size) d[(*d_len)++] = '-';
		n *= -1;
	}

    // Insert fractional and integer part
    for (j=0; j<np; j++){
        if (s_len<S_SIZE) s[s_len++] = (n % 10) + '0';
		n /= 10;
    }
    if ((s_len<S_SIZE)&&np) s[s_len++] = '.';
	do{
        if (s_len<S_SIZE) s[s_len++] = (n % 10) + '0';
		n /= 10;
	}while(n);

    // Reverse copy back
	for (j=0; (j<s_len)&&(*d_len<d_size); j++){
        d[(*d_len)++] = s[s_len-1-j];
    }
}

uint8_t str_atoul(uint8_t* s, uint16_t s_len, uint32_t* n){
	uint8_t s_pos = 0;
	*n = 0;
	for (; s_pos<s_len; s_pos++){
		if ( (s[s_pos] < '0') || (s[s_pos] > '9') ) return 0;
        *n *= 10;
        *n += s[s_pos] - '0';
	}
	return 1;
}

// Similar to the above, but for non-terminated strings
uint8_t str_atoil(uint8_t* s, uint16_t s_len, int32_t* n){
	uint8_t s_pos = 0;
	*n = 0;
	if (s[0] == '-') s_pos++;	// Skip the minus
	for(; s_pos<s_len; s_pos++){
		if ( (s[s_pos] < '0') || (s[s_pos] > '9') ) return 0;
        *n *= 10;
        *n += s[s_pos] - '0';
	}
	if (s[0] == '-') *n *= -1;
	return 1;
}

void str_htoal(uint8_t* s, uint16_t* s_len, uint32_t n, uint8_t n_len){
	uint8_t j;
	uint8_t temp;
	uint8_t bitshift = (n_len-1)*4;
	uint32_t mask = 0xF << bitshift;
	for (j=0; j<n_len; j++){
		temp = (n & mask) >> bitshift;
		n <<= 4;
		if(temp < 10) s[(*s_len)++] = (temp + '0');
		else s[(*s_len)++] = (temp + 55);
	}
}

uint8_t str_time(uint8_t* out, uint16_t out_size, uint32_t sec){
	const uint32_t divi[] = {86400, 3600, 60, 1};
	const uint8_t divi_name[] = {'d', 'h', 'm', 's'};
	uint8_t j, k, len;
	uint32_t n;
	uint8_t str[10];
	uint16_t out_len = 0;
	
	for (j=0; j<4; j++){
		if (sec >= divi[j]){
			n = sec/divi[j];
			sec %= divi[j];
			len = 0;
			do{
				str[len++] = (n % 10) + '0';
				n /= 10;
			}while(n);
			for (k=0; k<len; k++){
				if (out_len<out_size) out[out_len++] = str[len-1-k];
			}
			if (out_len<out_size) out[out_len++] = divi_name[j];
			//if (sec) term_cmd_put(' ');
		}
	}
	if (out_len<out_size) out[out_len] = 0;
	else out[0] = 0; // Ensure output gets terminated!
	return out_len;
}
