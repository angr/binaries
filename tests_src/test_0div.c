int main() {
	int i = 0;
	while (i < 10000) i++;
	i /= 0;
	return i;
}
