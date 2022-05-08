# fucking lisp
# lob aka lot of brackets and also lobotomy because that's what it feels like programming in this
import string


class TOKEN:
	def __init__(self, c, type):
		self.c = c
		self.type = type


	def __eq__(self, cond):
		if (self.type == "IDENTIFIER"):
			return self.type == cond
			
		return self.c == cond

	def __str__(self):
		return f"{self.c} : {self.type}"

	def __repr__(self):
		return self.__str__()


def tokenize(text):
	tokens = []
	token = ""
	number = False
	for c in text:
		if (c == "("):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False
			tokens.append(TOKEN(c, "LPAREN"))

		elif (c == ")"):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False
			tokens.append(TOKEN(c, "RPAREN"))

		elif (c in string.ascii_letters):
			token += c

		elif (c in string.digits):
			if (token):
				token += c
			else:
				number = True
				token += c

		elif (c == " "):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False

		elif (c == "\n"):
			if (token):
				if (number):
					token = int(token)
				tokens.append(TOKEN(token, "IDENTIFIER"))
				token = ""
				number = False

			tokens.append(TOKEN(c, "NEWLINE"))

		# elif (c in "+-*/%"):


	return tokens


def empty_expr():
	return {
		"keyword" : "",
		"specifier" : "",
		"args" : [],
	}

def parse(tokens):

	expr = empty_expr()

	p_count = 0
	i = 0
	line = 1
	column = 0
	args = False
	arg_p_count = None
	while i < len(tokens):
		t = tokens[i]
	# for t in tokens:
		if (t == "("):
			if (args):
				tmp_expr, tmp_i = parse(tokens[i:])
				i += tmp_i
				expr["args"].append(tmp_expr)
				
			elif (expr["keyword"]):
				arg_p_count = p_count
				args = True

			else:
				p_count += 1

		elif (t == ")"):
			p_count -= 1
			if (arg_p_count == p_count):
				arg_p_count = None
				args = False
				# print(expr)

			if (p_count == 0):
				# return expr, i
				break
				# eval_expr(expr)
				# expr = empty_expr()


		elif (t == "\n"):
			column = 0
			line += 1


		elif (t == "IDENTIFIER"):
			if (args):
				expr["args"].append(t.c)

			else:
				if (expr["keyword"]):
					expr["specifier"] = t.c

				else:
					expr["keyword"] = t.c

		if (p_count < 0):
			 raise Exception(f"too many right parenthesis {line}.{i}")

		i += 1
		column += 1

	if (p_count > 0):
		 raise Exception(f"too many left parenthesis {line}.{i}")

	return expr, i



def eval_expr(expr):
	# for 
	# if (expr["keyword"] == "add"):
		# print(inbuilt_add(expr["args"]))
		# print(expr, inbuilt_add(expr["args"]))


def inbuilt_add(args):
	return sum(args)


t = """
(add t (2 (add (4 4))))
"""
# t = """
# (add t(2 2))
# """
print("lex: ", tokenize(t))
print("parse: ", parse(tokenize(t)))


