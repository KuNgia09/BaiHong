	.text
	.align	2
	.global	decrypt
	.type	decrypt, %function
	/* �ַ������ܺ��� 
	* r0:Ҫ���ܵ��ֽ����飬��0��β
	*/
decrypt:
	stmfd	sp!, {r1, lr}
	ldrb r1, [r0] 	@ȡһ���ֽ�
	cmp r1, #0		@��Ϊ0�ͽ���ѭ������
	beq L2			@������󷵻�
L1:
	mvns r1, r1		@�ֽ�ȡ��
	strb r1, [r0]	@����д��ȥ
	adds r0, #1		@�ƶ��ֽ�����ָ��
	ldrb r1, [r0]	@ȡ��һ���ֽ�
	cmp r1, #0		@��Ϊ0�ͽ���ѭ������
	bne L1			@ѭ������
L2:
	ldmfd	sp!, {r1, pc}
	
