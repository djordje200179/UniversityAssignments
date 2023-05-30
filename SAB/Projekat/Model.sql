CREATE TABLE [Article]
( 
	[idArticle]          integer  IDENTITY  NOT NULL ,
	[idShop]             integer  NOT NULL ,
	[price]              integer  NOT NULL ,
	[quantity]           integer  NOT NULL 
	CONSTRAINT [Zero_1444337872]
		 DEFAULT  0,
	[name]               varchar(100)  NOT NULL 
)
go

CREATE TABLE [Buyer]
( 
	[idBuyer]            integer  IDENTITY  NOT NULL ,
	[balance]            decimal(10,3)  NOT NULL 
	CONSTRAINT [Zero_1890300070]
		 DEFAULT  0,
	[idCity]             integer  NOT NULL ,
	[name]               varchar(100)  NOT NULL 
)
go

CREATE TABLE [City]
( 
	[idCity]             integer  IDENTITY  NOT NULL ,
	[name]               varchar(100)  NOT NULL 
)
go

CREATE TABLE [Order]
( 
	[idOrder]            integer  IDENTITY  NOT NULL ,
	[idBuyer]            integer  NOT NULL ,
	[state]              varchar(8)  NOT NULL 
	CONSTRAINT [CreatedOrder_50554222]
		 DEFAULT  'created'
	CONSTRAINT [OrderState_252841714]
		CHECK  ( [state]='created' OR [state]='sent' OR [state]='arrived' ),
	[idCity]             integer  NULL 
	CONSTRAINT [NULL_142566259]
		 DEFAULT  NULL,
	[daysLeft]           integer  NULL 
	CONSTRAINT [Zero_2106636312]
		 DEFAULT  0,
	[systemDiscount]     bit  NULL 
	CONSTRAINT [Zero_411394159]
		 DEFAULT  0,
	[path]               varchar(1000)  NULL ,
	[sentTime]           datetime  NULL ,
	[receivedTime]       datetime  NULL ,
	[price]              decimal(10,3)  NULL 
)
go

CREATE TABLE [OrderArticle]
( 
	[idArticle]          integer  NOT NULL ,
	[idOrder]            integer  NOT NULL ,
	[price]              integer  NULL ,
	[discount]           integer  NULL ,
	[quantity]           integer  NOT NULL ,
	[daysLeft]           integer  NOT NULL 
	CONSTRAINT [Zero_910398870]
		 DEFAULT  0
)
go

CREATE TABLE [OrderPayment]
( 
	[idTransaction]      integer  NOT NULL 
)
go

CREATE TABLE [Road]
( 
	[length]             integer  NOT NULL ,
	[idCity1]            integer  NOT NULL ,
	[idCity2]            integer  NOT NULL 
)
go

CREATE TABLE [Shop]
( 
	[idShop]             integer  IDENTITY  NOT NULL ,
	[balance]            decimal(10,3)  NOT NULL 
	CONSTRAINT [Zero_747216816]
		 DEFAULT  0,
	[idCity]             integer  NOT NULL ,
	[discount]           integer  NOT NULL 
	CONSTRAINT [Zero_2119384137]
		 DEFAULT  0,
	[name]               varchar(100)  NOT NULL 
)
go

CREATE TABLE [ShopTransfer]
( 
	[idTransaction]      integer  NOT NULL ,
	[idShop]             integer  NOT NULL 
)
go

CREATE TABLE [Transaction]
( 
	[idTransaction]      integer  IDENTITY  NOT NULL ,
	[paymentTime]        datetime  NOT NULL ,
	[idOrder]            integer  NOT NULL ,
	[amount]             decimal(10,3)  NOT NULL 
)
go

ALTER TABLE [Article]
	ADD CONSTRAINT [XPKArticle] PRIMARY KEY  CLUSTERED ([idArticle] ASC)
go

ALTER TABLE [Buyer]
	ADD CONSTRAINT [XPKBuyer] PRIMARY KEY  CLUSTERED ([idBuyer] ASC)
go

ALTER TABLE [City]
	ADD CONSTRAINT [XPKCity] PRIMARY KEY  CLUSTERED ([idCity] ASC)
go

ALTER TABLE [Order]
	ADD CONSTRAINT [XPKOrder] PRIMARY KEY  CLUSTERED ([idOrder] ASC)
go

ALTER TABLE [OrderArticle]
	ADD CONSTRAINT [XPKOrderArticle] PRIMARY KEY  CLUSTERED ([idArticle] ASC,[idOrder] ASC)
go

ALTER TABLE [OrderPayment]
	ADD CONSTRAINT [XPKOrderPayment] PRIMARY KEY  CLUSTERED ([idTransaction] ASC)
go

ALTER TABLE [Road]
	ADD CONSTRAINT [XPKRoad] PRIMARY KEY  CLUSTERED ([idCity1] ASC,[idCity2] ASC)
go

ALTER TABLE [Shop]
	ADD CONSTRAINT [XPKShop] PRIMARY KEY  CLUSTERED ([idShop] ASC)
go

ALTER TABLE [ShopTransfer]
	ADD CONSTRAINT [XPKShopTransfer] PRIMARY KEY  CLUSTERED ([idTransaction] ASC)
go

ALTER TABLE [Transaction]
	ADD CONSTRAINT [XPKTransaction] PRIMARY KEY  CLUSTERED ([idTransaction] ASC)
go


ALTER TABLE [Article]
	ADD CONSTRAINT [R_8] FOREIGN KEY ([idShop]) REFERENCES [Shop]([idShop])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go


ALTER TABLE [Buyer]
	ADD CONSTRAINT [R_17] FOREIGN KEY ([idCity]) REFERENCES [City]([idCity])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go


ALTER TABLE [Order]
	ADD CONSTRAINT [R_10] FOREIGN KEY ([idBuyer]) REFERENCES [Buyer]([idBuyer])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go

ALTER TABLE [Order]
	ADD CONSTRAINT [R_21] FOREIGN KEY ([idCity]) REFERENCES [City]([idCity])
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go


ALTER TABLE [OrderArticle]
	ADD CONSTRAINT [R_11] FOREIGN KEY ([idArticle]) REFERENCES [Article]([idArticle])
		ON DELETE CASCADE
		ON UPDATE NO ACTION
go

ALTER TABLE [OrderArticle]
	ADD CONSTRAINT [R_12] FOREIGN KEY ([idOrder]) REFERENCES [Order]([idOrder])
		ON DELETE CASCADE
		ON UPDATE NO ACTION
go


ALTER TABLE [OrderPayment]
	ADD CONSTRAINT [R_26] FOREIGN KEY ([idTransaction]) REFERENCES [Transaction]([idTransaction])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go


ALTER TABLE [Road]
	ADD CONSTRAINT [R_6] FOREIGN KEY ([idCity2]) REFERENCES [City]([idCity])
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go

ALTER TABLE [Road]
	ADD CONSTRAINT [R_5] FOREIGN KEY ([idCity1]) REFERENCES [City]([idCity])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go


ALTER TABLE [Shop]
	ADD CONSTRAINT [R_7] FOREIGN KEY ([idCity]) REFERENCES [City]([idCity])
		ON DELETE NO ACTION
		ON UPDATE NO ACTION
go


ALTER TABLE [ShopTransfer]
	ADD CONSTRAINT [R_28] FOREIGN KEY ([idTransaction]) REFERENCES [Transaction]([idTransaction])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go

ALTER TABLE [ShopTransfer]
	ADD CONSTRAINT [R_29] FOREIGN KEY ([idShop]) REFERENCES [Shop]([idShop])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go


ALTER TABLE [Transaction]
	ADD CONSTRAINT [R_22] FOREIGN KEY ([idOrder]) REFERENCES [Order]([idOrder])
		ON DELETE CASCADE
		ON UPDATE CASCADE
go
