// <auto-generated/>
using System;

namespace Telegram.Api.TL
{
	public partial class TLInputFile : TLInputFileBase 
	{
		public String Md5Checksum { get; set; }

		public TLInputFile() { }
		public TLInputFile(TLBinaryReader from)
		{
			Read(from);
		}

		public override TLType TypeId { get { return TLType.InputFile; } }

		public override void Read(TLBinaryReader from)
		{
			Id = from.ReadInt64();
			Parts = from.ReadInt32();
			Name = from.ReadString();
			Md5Checksum = from.ReadString();
		}

		public override void Write(TLBinaryWriter to)
		{
			to.Write(0xF52FF27F);
			to.Write(Id);
			to.Write(Parts);
			to.Write(Name);
			to.Write(Md5Checksum);
		}
	}
}